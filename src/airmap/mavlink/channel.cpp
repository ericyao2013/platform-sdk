#include <airmap/mavlink/channel.h>

airmap::mavlink::Channel::Subscription airmap::mavlink::Channel::subscribe(const Subscriber& subscriber) {
  std::lock_guard<std::mutex> lg{guard_};
  return subscribers_.insert(subscribers_.end(), subscriber);
}

void airmap::mavlink::Channel::unsubscribe(Subscription&& subscription) {
  std::lock_guard<std::mutex> lg{guard_};
  subscribers_.erase(subscription);
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