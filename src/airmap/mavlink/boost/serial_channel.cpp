#include <airmap/mavlink/boost/serial_channel.h>

namespace {

std::exception_ptr wrap_error_code(const ::boost::system::error_code& ec) {
  return std::make_exception_ptr(std::runtime_error{ec.message()});
}

constexpr const char* component{"airmap::mavlink::SerialChannel"};

}  // namespace

airmap::mavlink::boost::SerialChannel::SerialChannel(const std::shared_ptr<Logger>& logger,
                                                     const std::shared_ptr<::boost::asio::io_service>& io_service,
                                                     const std::string& device_file)
    : log_{logger}, io_service_{io_service}, serial_port_{*io_service_, device_file} {
  ::memset(&parse_buffer_.msg, 0, sizeof(parse_buffer_.msg));
  ::memset(&parse_buffer_.status, 0, sizeof(parse_buffer_.status));

  serial_port_.set_option(::boost::asio::serial_port::baud_rate(57600));
}

void airmap::mavlink::boost::SerialChannel::start() {
  using std::placeholders::_1;
  using std::placeholders::_2;
  serial_port_.async_read_some(::boost::asio::buffer(buffer_),
                               std::bind(&SerialChannel::handle_read, shared_from_this(), _1, _2));
}

void airmap::mavlink::boost::SerialChannel::cancel() {
  serial_port_.cancel();
}

void airmap::mavlink::boost::SerialChannel::handle_read(const ::boost::system::error_code& ec,
                                                        std::size_t transferred) {
  if (ec) {
    log_.errorf(component, "failed to read from serial device: %s", ec.message());
    return;
  }

  if (auto result = process_mavlink_data(transferred)) {
    invoke_subscribers(result.get());
  }

  start();
}

airmap::Optional<std::vector<mavlink_message_t>> airmap::mavlink::boost::SerialChannel::process_mavlink_data(
    std::size_t transferred) {
  Optional<std::vector<mavlink_message_t>> result;

  for (std::size_t i = 0; i < transferred; i++) {
    auto rc = mavlink_frame_char_buffer(&parse_buffer_.msg, &parse_buffer_.status, buffer_[i], &parse_out_.msg,
                                        &parse_out_.status);

    switch (rc) {
      case MAVLINK_FRAMING_INCOMPLETE:
        break;
      case MAVLINK_FRAMING_OK:
        if (not result)
          result = std::vector<mavlink_message_t>{};
        result.get().push_back(parse_out_.msg);
        break;
      case MAVLINK_FRAMING_BAD_CRC:
        break;
    }
  }
  return result;
}