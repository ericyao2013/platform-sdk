#ifndef AIRMAP_CLIENT_H_
#define AIRMAP_CLIENT_H_

#include <airmap/outcome.h>

#include <functional>
#include <memory>
#include <string>

namespace airmap {

class Aircrafts;
class Airspaces;
class Authenticator;
class DeviceIdentifier;
class Flights;
class SecretsStore;
class Telemetry;

/// Client enables applications to use the AirMap services and APIs.
class Client {
 public:
  /// Credentials bundles up all credentials required
  /// to use the AirMap SDK and APIs.
  struct Credentials {
    std::string api_key;
  };

  /// CreateResult models the result of creating a new client instance xor
  /// an exception describing why creation of an instance failed.
  using CreateResult = Outcome<std::shared_ptr<Client>, std::exception_ptr>;

  /// CreateCallback is invoked when the creation of a client finishes.
  using CreateCallback = std::function<void(const CreateResult&)>;

  /// create_with_credentials returns a new Client instance for 'credentials'
  /// or nullptr in case of issues.
  ///
  /// TODO(tvoss): Depending on the scope of this core SDK, determine whether
  /// the embedded libc++ used by NuttX properly supports std::shared_ptr.
  static void create_with_credentials(const Credentials& credentials,
                                      const std::shared_ptr<SecretsStore>& secrets_store,
                                      const std::shared_ptr<DeviceIdentifier>& device_identifier,
                                      const CreateCallback& cb);

  Client(const Client&) = delete;
  Client(Client&&)      = delete;
  virtual ~Client();
  Client& operator=(const Client&) = delete;
  Client& operator=(Client&&) = delete;

  /// TODO(tvoss): Both run() and stop() naturally trigger a conversation
  /// about integration with alien event loops and threading models. For the
  /// time being we will enable calling code to hand a thread of execution to the client
  /// and make sure that all callbacks are only ever executed on the thread handed to 'run()'.
  ///
  /// run hands a thread of execution to a 'Client' instance
  /// and implementations should not return from it until stop
  /// is called.
  virtual void run() = 0;

  /// stop requests a 'Client' instance to finish up outstanding operations
  /// and return from its 'run()' method implementation.
  virtual void stop() = 0;

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
  Client();
};

}  // namespace airmap

#endif  // AIRMAP_CLIENT_H_
