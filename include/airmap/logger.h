#ifndef AIRMAP_LOGGER_H_
#define AIRMAP_LOGGER_H_

#include <airmap/do_not_copy_or_move.h>

#include <iostream>
#include <memory>

namespace airmap {

/// Logger abstracts logging of human-readable message
/// providing details on the operation of the system.
class Logger : DoNotCopyOrMove {
 public:
  /// Severity enumerates all known levels of severity
  enum class Severity { info, error };

  /// info logs a message from component with Severity::info.
  void info(const char* message, const char* component);

  /// error logs a message from component with Severity::error.
  void error(const char* message, const char* component);

  /// log handles the incoming log message originating from component.
  /// Implementation should handle the case of component being a nullptr
  /// gracefully.
  virtual void log(Severity severity, const char* message, const char* component) = 0;

 protected:
  Logger() = default;
};

/// create_default_logger returns a Logger implementation writing
/// log messages to 'out'.
std::shared_ptr<Logger> create_default_logger(std::ostream& out = std::cerr);

/// create_null_logger returns a logger that does the equivalent of
/// > /dev/null.
std::shared_ptr<Logger> create_null_logger();

}  // namespace airmap

#endif  // AIRMAP_LOGGER_H_