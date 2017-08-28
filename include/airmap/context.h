#ifndef AIRMAP_CONTEXT_H_
#define AIRMAP_CONTEXT_H_

#include <airmap/client.h>
#include <airmap/do_not_copy_or_move.h>
#include <airmap/logger.h>
#include <airmap/outcome.h>

#include <functional>
#include <unordered_set>

namespace airmap {

class Context : DoNotCopyOrMove {
 public:
  enum class ReturnCode { success = 0, error = 1 };

  using ClientCreateResult   = Outcome<std::shared_ptr<Client>, std::exception_ptr>;
  using ClientCreateCallback = std::function<void(const ClientCreateResult&)>;
  using CreateResult         = Outcome<std::shared_ptr<Context>, std::exception_ptr>;
  using SignalHandler        = std::function<void(int)>;
  using SignalSet            = std::unordered_set<int>;

  /// create tries to assemble and return a new Context instance.
  static CreateResult create(const std::shared_ptr<Logger>& logger);

  /// create_client_with_configuration schedules creation of a new client with 'configuration'
  /// and reports results to 'cb'.
  virtual void create_client_with_configuration(const Client::Configuration& configuration,
                                                const ClientCreateCallback& cb) = 0;

  /// exec hands a thread of execution to a Context instance, monitoring
  /// the signals present in 'signal_set' and dispatching incoming signals
  /// to the registered handlers.
  ///
  /// Implementations are expected to block the current thread until
  /// either an error occured or the user explicitly requests a Context
  /// instance to stop.
  virtual ReturnCode exec(const SignalSet& signal_set, const SignalHandler& handler) = 0;

  /// run hands a thread of execution to the context.
  ///
  /// Implementations are expected to block the current thread until
  /// either an error occured or the user explicitly requests a Context
  /// instance to stop.
  virtual ReturnCode run() = 0;

  /// stop requests an instance to shut down its operation and return from
  /// run.
  virtual void stop(ReturnCode rc = ReturnCode::success) = 0;

 protected:
  Context() = default;
};

}  // namespace airmap

#endif  // AIRMAP_CONTEXT_H_