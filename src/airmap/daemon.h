#ifndef AIRMAP_DAEMON_H_
#define AIRMAP_DAEMON_H_

#include <airmap/client.h>
#include <airmap/result.h>

#include <memory>

namespace airmap {

class MissionPlanner;
class TelemetrySource;

/// Daemon respresents AirMap and all of its services within
/// the system. It listens to incoming data (both in terms of
/// sensors as well as in terms of control messages) and enables
/// a drone operation platform to deeply integrate airspace intelligence
/// in its operations.
class Daemon {
public:
    /// Configuration contains all runtime dependencies of Daemon.
    struct Configuration {
        static Result<Configuration, std::exception_ptr> from_command_line(int argc, char **argv);

        std::shared_ptr<TelemetrySource> telemetry_source;
        std::shared_ptr<MissionPlanner> mission_planner;
    };

    /// main assembles a Configuration from command line
    /// arguments, environment variables and configuration files
    /// and executes the daemon.
    ///
    /// airmapd help
    ///   --device-identifier=identifier one of {random, mac}
    static int main(int argc, char** argv);

    /// Daemon initializes a new instance with the functional
    /// dependencies provided via 'configuration'.
    explicit Daemon(Configuration&& configuration);

    /// run executes the daemon, blocking the current thread until
    /// either stop() is called or an error occurs.
    void run();

    /// stop requests the Daemon instance to shut down and return
    /// from a previous call to run.
    void stop();

private:
    std::unique_ptr<Client> client;
    std::shared_ptr<TelemetrySource> telemetry_source;
    std::shared_ptr<MissionPlanner> mission_planner;
};

}

#endif  // AIRMAP_DAEMON_H_
