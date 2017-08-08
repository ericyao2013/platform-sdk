#include <airmap/mavlink/channel.h>

#include <iostream>

airmap::mavlink::Channel::Channel() {
  ::memset(&parse_buffer_.msg, 0, sizeof(parse_buffer_.msg));
  ::memset(&parse_buffer_.status, 0, sizeof(parse_buffer_.status));
}

const airmap::mavlink::Channel::Counters& airmap::mavlink::Channel::counters() const {
  return counters_;
}

airmap::mavlink::Channel::Subscription airmap::mavlink::Channel::subscribe(const Subscriber& subscriber) {
  std::lock_guard<std::mutex> lg{guard_};
  return subscribers_.insert(subscribers_.end(), subscriber);
}

void airmap::mavlink::Channel::unsubscribe(Subscription&& subscription) {
  std::lock_guard<std::mutex> lg{guard_};
  subscribers_.erase(subscription);
}

void airmap::mavlink::Channel::start() {
  counters_.received = 0;
  counters_.good     = 0;
  counters_.bad      = 0;

  start_impl();
}

void airmap::mavlink::Channel::stop() {
  stop_impl();
}

void airmap::mavlink::Channel::invoke_subscribers(const std::vector<mavlink_message_t>& msgs) {
  std::unique_lock<std::mutex> ul{guard_};
  auto subscribers = subscribers_;
  ul.unlock();

  for (const auto& msg : msgs) {
    for (const auto& subscriber : subscribers)
      subscriber(msg);
  }
}

airmap::Optional<std::vector<mavlink_message_t>> airmap::mavlink::Channel::process_mavlink_data(const char* begin,
                                                                                                const char* end) {
  Optional<std::vector<mavlink_message_t>> result;

  for (; begin < end; ++begin) {
    auto rc = mavlink_frame_char_buffer(&parse_buffer_.msg, &parse_buffer_.status, *begin, &parse_out_.msg,
                                        &parse_out_.status);

    switch (rc) {
      case MAVLINK_FRAMING_INCOMPLETE:
        break;
      case MAVLINK_FRAMING_OK:
        if (not result)
          result = std::vector<mavlink_message_t>{};
        result.get().push_back(parse_out_.msg);
        counters_.received++;
        counters_.good++;
        break;
      case MAVLINK_FRAMING_BAD_CRC:
        counters_.received++;
        counters_.bad++;
        break;
    }
  }

  return result;
}