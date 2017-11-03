#ifndef AIRMAP_MAVLINK_BOOST_TCP_ROUTE_H_
#define AIRMAP_MAVLINK_BOOST_TCP_ROUTE_H_

#include <airmap/mavlink/router.h>

#include <airmap/logger.h>
#include <airmap/util/formatting_logger.h>

#include <boost/asio.hpp>

#include <array>
#include <memory>
#include <queue>
#include <set>

namespace airmap {
namespace mavlink {
namespace boost {

// TcpRoute forwards all incoming messages to connected TCP clients.
class TcpRoute : public Router::Route, public std::enable_shared_from_this<TcpRoute> {
 public:
  // create initializes and returns a new instance with 'io_service', 'endpoint' and 'logger'.
  static std::shared_ptr<TcpRoute> create(const std::shared_ptr<::boost::asio::io_service>& io_service,
                                          const ::boost::asio::ip::tcp::endpoint& endpoint,
                                          const std::shared_ptr<Logger>& logger);

  // From Route
  void process(const mavlink_message_t& message) override;
  void start() override;
  void stop() override;

 private:
  // Session models an active session with a remote endpoint.
  class Session : public std::enable_shared_from_this<Session> {
   public:
    // create initializes and returns a new instance with 'io_service' and 'logger'.
    static std::shared_ptr<Session> create(const std::shared_ptr<::boost::asio::io_service>& io_service,
                                           const std::shared_ptr<Logger>& logger);

    void process(const mavlink_message_t& message);
    ::boost::asio::ip::tcp::socket& socket();

   private:
    // Session initializes a new instance with 'io_service' and 'logger'.
    explicit Session(const std::shared_ptr<::boost::asio::io_service>& io_service,
                     const std::shared_ptr<Logger>& logger);

    void process();

    std::shared_ptr<::boost::asio::io_service> io_service_;
    util::FormattingLogger log_;
    ::boost::asio::ip::tcp::socket socket_;
    std::queue<std::array<unsigned char, 512>> buffers_;
  };

  // TcpRoute initializes a new instance with 'context'.
  explicit TcpRoute(const std::shared_ptr<::boost::asio::io_service>& io_service,
                    const ::boost::asio::ip::tcp::endpoint& endpoint, const std::shared_ptr<Logger>& logger);

  void handle_accept(const ::boost::system::error_code& ec);

  std::shared_ptr<::boost::asio::io_service> io_service_;
  ::boost::asio::ip::tcp::acceptor acceptor_;
  util::FormattingLogger log_;
  std::set<std::shared_ptr<Session>> sessions_;
};

}  // namespace boost
}  // namespace mavlink
}  // namespace airmap

#endif  // AIRMAP_MAVLINK_BOOST_TCP_ROUTE_H_
