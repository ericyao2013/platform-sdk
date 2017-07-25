#ifndef AIRMAP_CONTEXT_H_
#define AIRMAP_CONTEXT_H_

#include <airmap/do_not_copy_or_move.h>

namespace airmap {

class Context : DoNotCopyOrMove {
 public:
  virtual void run()  = 0;
  virtual void stop() = 0;

 protected:
  Context() = default;
};

}  // namespace airmap

#endif  // AIRMAP_CONTEXT_H_