#ifndef AIRMAP_CONTEXT_H_
#define AIRMAP_CONTEXT_H_

#include <airmap/client.h>
#include <airmap/do_not_copy_or_move.h>
#include <airmap/logger.h>
#include <airmap/outcome.h>

#include <functional>

namespace airmap {

class Context : DoNotCopyOrMove {
 public:
  using ClientCreateResult   = Outcome<std::shared_ptr<Client>, std::exception_ptr>;
  using ClientCreateCallback = std::function<void(const ClientCreateResult&)>;
  using CreateResult         = Outcome<std::shared_ptr<Context>, std::exception_ptr>;

  /// create tries to assemble and return a new Context instance.
  static CreateResult create(const std::shared_ptr<Logger>& logger);

  /// create_client_with_credentials schedules creation of a new client with 'credentials'
  /// and reports results to 'cb'.
  virtual void create_client_with_credentials(const Client::Credentials& credentials,
                                              const ClientCreateCallback& cb) = 0;

  /// run hands a thread of execution to the context.
  virtual void run() = 0;

  /// stop requests an instance to shut down its operation and return from
  /// run.
  virtual void stop() = 0;

 protected:
  Context() = default;
};

}  // namespace airmap

#endif  // AIRMAP_CONTEXT_H_