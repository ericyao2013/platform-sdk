#ifndef AIRMAP_MAVLINK_BOOST_SERIAL_CHANNEL_H_
#define AIRMAP_MAVLINK_BOOST_SERIAL_CHANNEL_H_

#include <airmap/logger.h>
#include <airmap/mavlink/channel.h>
#include <airmap/optional.h>
#include <airmap/util/formatting_logger.h>

#include <boost/asio.hpp>

namespace airmap {
namespace mavlink {
namespace boost {

class SerialChannel : public Channel, public std::enable_shared_from_this<SerialChannel> {
 public:
  static constexpr std::size_t buffer_size{1024};

  explicit SerialChannel(const std::shared_ptr<Logger>& logger,
                         const std::shared_ptr<::boost::asio::io_service>& io_service, const std::string& device_file);

  // From Channel
  void start() override;
  void cancel() override;

 private:
  void handle_read(const ::boost::system::error_code& ec, std::size_t transferred);
  Optional<std::vector<mavlink_message_t>> process_mavlink_data(std::size_t transferred);

  util::FormattingLogger log_;
  std::shared_ptr<::boost::asio::io_service> io_service_;
  ::boost::asio::serial_port serial_port_;
  std::array<char, buffer_size> buffer_;

  struct {
    mavlink_message_t msg;
    mavlink_status_t status;
  } parse_buffer_;
  struct {
    mavlink_message_t msg;
    mavlink_status_t status;
  } parse_out_;
};

}  // namespace boost
}  // namespace mavlink
}  // namespace airmap

#endif  // AIRMAP_MAVLINK_BOOST_SERIAL_CHANNEL_H_