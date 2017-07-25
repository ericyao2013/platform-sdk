#ifndef AIRMAP_CLIENT_H_
#define AIRMAP_CLIENT_H_

#include <airmap/context.h>
#include <airmap/do_not_copy_or_move.h>
#include <airmap/outcome.h>

#include <functional>
#include <memory>
#include <string>

namespace airmap {

class Aircrafts;
class Airspaces;
class Authenticator;
class Flights;
class Telemetry;

/// Client enables applications to use the AirMap services and APIs.
class Client : DoNotCopyOrMove {
 public:
  /// Credentials bundles up all credentials required
  /// to use the AirMap SDK and APIs.
  struct Credentials {
    std::string api_key;
  };

  /// ContextResult models the synchronous result of creating a client.
  using ContextResult = Outcome<std::shared_ptr<Context>, std::exception_ptr>;

  /// CreateResult models the async result of creating a new client instance xor
  /// an exception describing why creation of an instance failed.
  using CreateResult = Outcome<std::shared_ptr<Client>, std::exception_ptr>;

  /// CreateCallback is invoked when the creation of a client finishes.
  using CreateCallback = std::function<void(const CreateResult&)>;

  /// create_with_credentials returns a new Client instance for 'credentials'
  /// or nullptr in case of issues.
  ///
  /// TODO(tvoss): Depending on the scope of this core SDK, determine whether
  /// the embedded libc++ used by NuttX properly supports std::shared_ptr.
  static ContextResult create_with_credentials(const Credentials& credentials,
                                               const CreateCallback& cb);

  /// authenticator returns the Authenticator implementation provided by the client.
  virtual Authenticator& authenticator() = 0;

  /// aircrafts returns the Aircrafts implementation provided by the client.
  virtual Aircrafts& aircrafts() = 0;

  /// airspaces returns the Airspaces implementation provided by the client.
  virtual Airspaces& airspaces() = 0;

  /// flights returns the Flights implementation provided by the client.
  virtual Flights& flights() = 0;

  /// telemetry returns the Telemetry implementation provided by the client.
  virtual Telemetry& telemetry() = 0;

 protected:
  Client() = default;
};

}  // namespace airmap

#endif  // AIRMAP_CLIENT_H_
