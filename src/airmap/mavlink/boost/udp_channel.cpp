#include <airmap/mavlink/boost/udp_channel.h>

namespace {

std::exception_ptr wrap_error_code(const ::boost::system::error_code& ec) {
  return std::make_exception_ptr(std::runtime_error{ec.message()});
}

constexpr const char* component{"airmap::mavlink::UdpChannel"};

}  // namespace

airmap::mavlink::boost::UdpChannel::UdpChannel(const std::shared_ptr<Logger>& logger,
                                                     const std::shared_ptr<::boost::asio::io_service>& io_service,
                                                     const ::boost::asio::ip::address& ip, std::uint16_t port)
    : log_{logger}, io_service_{io_service}, endpoint_{ip, port}, socket_{*io_service_} {
}

void airmap::mavlink::boost::UdpChannel::start() {
  auto sp = shared_from_this();
  socket_.async_receive_from(::boost::asio::buffer(buffer_), endpoint_, [this, sp](const auto& ec, auto size) {
    if (ec) {
      log_.errorf(component, "failed to read from udp endpoint: %s", ec.message());
      return;
    }

    if (auto result = handle_read(size))
      invoke_subscribers(result.get());

    start();
  });
}

void airmap::mavlink::boost::UdpChannel::cancel() {
  socket_.cancel();
}

airmap::Optional<std::vector<mavlink_message_t>> airmap::mavlink::boost::UdpChannel::handle_read(
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