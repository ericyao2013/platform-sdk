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
}

void airmap::mavlink::boost::SerialChannel::start() {
  auto sp = shared_from_this();
  serial_port_.async_read_some(::boost::asio::buffer(buffer_), [this, sp](const auto& ec, auto size) {
    if (ec) {
      log_.errorf(component, "failed to read from serial device: %s", ec.message());
      return;
    }

    if (auto result = handle_read(size))
      invoke_subscribers(result.get());

    start();
  });
}

void airmap::mavlink::boost::SerialChannel::cancel() {
  serial_port_.cancel();
}

airmap::Optional<std::vector<mavlink_message_t>> airmap::mavlink::boost::SerialChannel::handle_read(
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