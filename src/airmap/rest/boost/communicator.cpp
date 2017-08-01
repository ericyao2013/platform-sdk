#include <airmap/rest/boost/communicator.h>

#include <airmap/rest/client.h>

#include <boost/beast/http.hpp>

#include <iostream>

namespace asio = boost::asio;
namespace http = boost::beast::http;
namespace ssl  = boost::asio::ssl;
using tcp      = boost::asio::ip::tcp;

namespace {
std::exception_ptr wrap_error_code(const boost::system::error_code& ec) {
  return std::make_exception_ptr(std::runtime_error{ec.message()});
}

constexpr const char* component{"rest::glib::Communicator"};
}  // namespace

airmap::rest::boost::Communicator::Communicator(const std::shared_ptr<Logger>& logger)
    : log_{logger},
      io_service_{std::make_shared<asio::io_service>()},
      keep_alive_{std::make_shared<asio::io_service::work>(*io_service_)},
      ssl_context_{std::make_shared<ssl::context>(ssl::context::sslv23)} {
  ssl_context_->set_options(ssl::context::default_workarounds | ssl::context::no_sslv2 | ssl::context::no_sslv3);
  ssl_context_->set_verify_mode(ssl::verify_none);
  ssl_context_->set_verify_callback([](bool preverified, ssl::verify_context&) {
    std::cout << std::boolalpha << "preverified: " << preverified << std::endl;
    return true;
  });
}

airmap::rest::boost::Communicator::~Communicator() {
}

// From airmap::Context
void airmap::rest::boost::Communicator::create_client_with_credentials(const Client::Credentials& credentials,
                                                                       const ClientCreateCallback& cb) {
  cb(ClientCreateResult{std::make_shared<rest::Client>(credentials, shared_from_this())});
}

void airmap::rest::boost::Communicator::run() {
  io_service_->run();
}

void airmap::rest::boost::Communicator::stop() {
  io_service_->stop();
}

// From airmap::rest::Communicator
void airmap::rest::boost::Communicator::get(const std::string& host, const std::string& path,
                                            std::unordered_map<std::string, std::string>&& query,
                                            std::unordered_map<std::string, std::string>&& headers, DoCallback cb) {
  log_.infof(component, __PRETTY_FUNCTION__);
  auto session = std::make_shared<Session>(Session::Get{}, log_.logger(), io_service_, ssl_context_, host, path,
                                           std::move(query), std::move(headers), std::move(cb));
  session->start();
}
void airmap::rest::boost::Communicator::post(const std::string& host, const std::string& path,
                                             std::unordered_map<std::string, std::string>&& headers,
                                             const std::string& body, DoCallback cb) {
  log_.infof(component, __PRETTY_FUNCTION__);
  auto session = std::make_shared<Session>(Session::Post{}, log_.logger(), io_service_, ssl_context_, host, path, body,
                                           std::move(headers), std::move(cb));
  session->start();
}

void airmap::rest::boost::Communicator::send_udp(const std::string&, std::uint16_t, const std::string&) {
}

void airmap::rest::boost::Communicator::dispatch(const std::function<void()>& task) {
  log_.infof(component, __PRETTY_FUNCTION__);
  io_service_->dispatch(task);
}

airmap::rest::boost::Communicator::Session::Session(const Get&, const std::shared_ptr<Logger>& logger,
                                                    const std::shared_ptr<asio::io_service>& io_service,
                                                    const std::shared_ptr<asio::ssl::context>& ssl_context,
                                                    const std::string& host, const std::string& path,
                                                    std::unordered_map<std::string, std::string>&& query,
                                                    std::unordered_map<std::string, std::string>&& headers,
                                                    DoCallback cb)
    : log{logger},
      io_service{io_service},
      ssl_context{ssl_context},
      resolver{*io_service},
      socket{*io_service, *ssl_context},
      cb{cb} {
  network::uri_builder uri_builder;
  uri_builder.host(host).path(path);
  for (const auto& pair : query)
    uri_builder.append_query_key_value_pair(pair.first, pair.second);
  uri = uri_builder.uri();

  request.method(http::verb::get);
  request.target(fmt::sprintf("%s/%s", uri.path(), uri.query()));
  for (const auto& pair : headers)
    request.set(pair.first, pair.second);
  request.prepare_payload();
}

airmap::rest::boost::Communicator::Session::Session(
    const Post&, const std::shared_ptr<Logger>& logger, const std::shared_ptr<asio::io_service>& io_service,
    const std::shared_ptr<asio::ssl::context>& ssl_context, const std::string& host, const std::string& path,
    const std::string& body, std::unordered_map<std::string, std::string>&& headers, DoCallback cb)
    : log{logger},
      io_service{io_service},
      ssl_context{ssl_context},
      resolver{*io_service},
      socket{*io_service, *ssl_context},
      cb{cb} {
  network::uri_builder uri_builder;
  uri = uri_builder.host(host).path(path).uri();

  request.method(http::verb::post);
  request.target(fmt::sprintf("%s/%s", uri.path(), uri.query()));
  for (const auto& pair : headers)
    request.set(pair.first, pair.second);
  request.body = std::move(body);
  request.prepare_payload();
}

void airmap::rest::boost::Communicator::Session::start() {
  log.infof(component, "%s: %s %s", __PRETTY_FUNCTION__, uri.host().to_string(), uri.scheme().to_string());
  resolver.async_resolve({uri.host().to_string(), "https"}, std::bind(&Session::handle_resolve, shared_from_this(),
                                                                      std::placeholders::_1, std::placeholders::_2));
}

void airmap::rest::boost::Communicator::Session::handle_resolve(const ::boost::system::error_code& error,
                                                                tcp::resolver::iterator iterator) {
  log.infof(component, __PRETTY_FUNCTION__);

  if (error) {
    cb(DoResult(wrap_error_code(error)));
    return;
  }

  log.infof(component, "trying to connect to: %s", iterator->endpoint());

  socket.lowest_layer().async_connect(*iterator,
                                      std::bind(&Session::handle_connect, shared_from_this(), std::placeholders::_1));
}

void airmap::rest::boost::Communicator::Session::handle_connect(const ::boost::system::error_code& error) {
  log.infof(component, __PRETTY_FUNCTION__);
  if (error) {
    cb(DoResult(wrap_error_code(error)));
    return;
  }
  SSL_set_tlsext_host_name(socket.native_handle(), uri.host().data());
  socket.async_handshake(ssl::stream_base::client,
                         std::bind(&Session::handle_ssl_handshake, shared_from_this(), std::placeholders::_1));
}

void airmap::rest::boost::Communicator::Session::handle_ssl_handshake(const ::boost::system::error_code& error) {
  log.infof(component, __PRETTY_FUNCTION__);
  if (error) {
    log.errorf(component, "failed to complete ssl handshake: %s", error.message());
    cb(DoResult(wrap_error_code(error)));
    return;
  }

  http::async_write(socket, request, std::bind(&Session::handle_write, shared_from_this(), std::placeholders::_1));
}

void airmap::rest::boost::Communicator::Session::handle_write(const ::boost::system::error_code& error) {
  log.infof(component, __PRETTY_FUNCTION__);
  if (error) {
    cb(DoResult(wrap_error_code(error)));
    return;
  }

  http::async_read(socket, buffer, response,
                   std::bind(&Session::handle_read, shared_from_this(), std::placeholders::_1));
}

void airmap::rest::boost::Communicator::Session::handle_read(const ::boost::system::error_code& error) {
  log.infof(component, __PRETTY_FUNCTION__);
  if (error) {
    cb(DoResult(wrap_error_code(error)));
    return;
  }

  log.infof(component, __PRETTY_FUNCTION__);

  std::cout << response.body << std::endl;
  cb(DoResult{response.body});
}