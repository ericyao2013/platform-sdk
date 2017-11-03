#include <airmap/mavlink/boost/tcp_route.h>

namespace {
constexpr const char* component{"TcpRoute"};
}  // namespace

std::shared_ptr<airmap::mavlink::boost::TcpRoute> airmap::mavlink::boost::TcpRoute::create(
    const std::shared_ptr<::boost::asio::io_service>& io_service, const ::boost::asio::ip::tcp::endpoint& endpoint,
    const std::shared_ptr<Logger>& logger) {
  return std::shared_ptr<TcpRoute>{new TcpRoute{io_service, endpoint, logger}};
}

airmap::mavlink::boost::TcpRoute::TcpRoute(const std::shared_ptr<::boost::asio::io_service>& io_service,
                                           const ::boost::asio::ip::tcp::endpoint& endpoint,
                                           const std::shared_ptr<Logger>& logger)
    : io_service_{io_service}, acceptor_{*io_service_, endpoint}, log_{logger} {
}

// start starts accepting connections
void airmap::mavlink::boost::TcpRoute::start() {
  auto sp      = shared_from_this();
  auto session = Session::create(io_service_, log_.logger());

  acceptor_.async_accept(session->socket(), [sp, session](const auto& error) {
    if (error) {
      sp->log_.errorf(component, "error accepting incoming connection: %s", error.message());
    } else {
      sp->log_.infof(component, "accepting incoming connection from endpoint: %s:%d",
                     session->socket().remote_endpoint().address().to_string(),
                     session->socket().remote_endpoint().port());
      sp->sessions_.insert(session);
      sp->start();
    }
  });
}

// stop stops accepting connections
void airmap::mavlink::boost::TcpRoute::stop() {
  acceptor_.cancel();
}

// From Route
void airmap::mavlink::boost::TcpRoute::process(const mavlink_message_t& message) {
  io_service_->post([ sp = shared_from_this(), message ]() {
    sp->log_.infof(component, "handing message to %d sessions", sp->sessions_.size());
    for (const auto& session : sp->sessions_)
      session->process(message);
  });
}

std::shared_ptr<airmap::mavlink::boost::TcpRoute::Session> airmap::mavlink::boost::TcpRoute::Session::create(
    const std::shared_ptr<::boost::asio::io_service>& io_service, const std::shared_ptr<Logger>& logger) {
  return std::shared_ptr<Session>{new Session{io_service, logger}};
}

::boost::asio::ip::tcp::socket& airmap::mavlink::boost::TcpRoute::Session::socket() {
  return socket_;
}

void airmap::mavlink::boost::TcpRoute::Session::process(const mavlink_message_t& message) {
  log_.infof(component, "processing mavlink message for session on endpoint %s:%d",
             socket_.remote_endpoint().address().to_string(), socket_.remote_endpoint().port());

  std::array<unsigned char, 512> buffer;
  mavlink_msg_to_send_buffer(buffer.data(), &message);

  io_service_->post([ sp = shared_from_this(), buffer ]() {
    sp->buffers_.emplace(buffer);

    if (sp->buffers_.size() == 1)
      sp->process();
  });
}

void airmap::mavlink::boost::TcpRoute::Session::process() {
  const auto& buffer = buffers_.front();
  ::boost::asio::async_write(
      socket_, ::boost::asio::buffer(buffer.data(), buffer.size()),
      ::boost::asio::transfer_all(), [sp = shared_from_this()](const auto& error, auto bytes_transferred) {
        sp->buffers_.pop();

        if (error) {
          sp->log_.infof(component, "failed to process mavlink message for session on endpoint %s:%d: %s",
                         sp->socket_.remote_endpoint().address().to_string(), sp->socket_.remote_endpoint().port(),
                         error.message());

          return;
        }

        if (bytes_transferred != sp->buffers_.front().size()) {
          return;
        }

        if (!sp->buffers_.empty())
          sp->process();
      });
}

airmap::mavlink::boost::TcpRoute::Session::Session(const std::shared_ptr<::boost::asio::io_service>& io_service,
                                                   const std::shared_ptr<Logger>& logger)
    : io_service_{io_service}, log_{logger}, socket_{*io_service_} {
}
