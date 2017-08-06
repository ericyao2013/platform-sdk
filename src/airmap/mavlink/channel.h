#ifndef AIRMAP_MAVLINK_CHANNEL_H_
#define AIRMAP_MAVLINK_CHANNEL_H_

#include <airmap/do_not_copy_or_move.h>

#include <standard/mavlink.h>

#include <functional>
#include <list>
#include <mutex>
#include <set>
#include <stdexcept>
#include <vector>

namespace airmap {
namespace mavlink {

class Channel : DoNotCopyOrMove {
 public:
  using Subscriber    = std::function<void(const mavlink_message_t&)>;
  using SubscriberSet = std::list<Subscriber>;
  using Subscription  = SubscriberSet::const_iterator;

  Subscription subscribe(const Subscriber&);
  void unsubscribe(Subscription&& subscription);

  virtual void start()  = 0;
  virtual void cancel() = 0;

 protected:
  Channel() = default;

  void invoke_subscribers(const std::vector<mavlink_message_t>& msgs);

 private:
  std::mutex guard_;
  SubscriberSet subscribers_;
};

}  // namespace mavlink
}  // namespace airmap

#endif  // AIRMAP_MAVLINK_CHANNEL_H_