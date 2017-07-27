#include <airmap/logger.h>

#include <spdlog/async_logger.h>
#include <spdlog/sinks/ostream_sink.h>

namespace {

class DefaultLogger : public airmap::Logger {
 public:
  constexpr static std::size_t buffer_size_in_bytes{(1 << 9) * 1024};

  explicit DefaultLogger(std::ostream& out)
      : async_logger_{"airmap", std::make_shared<spdlog::sinks::ostream_sink_mt>(out), buffer_size_in_bytes} {
  }

  // From airmap::Logger
  void log(Severity severity, const char* message, const char* component) override {
    switch (severity) {
      case Severity::info:
        async_logger_.log(spdlog::level::info, message);
        break;
      case Severity::error:
        async_logger_.log(spdlog::level::err, message);
        break;
    }
  }

 private:
  spdlog::async_logger async_logger_;
};

}  // namespace

void airmap::Logger::info(const char* message, const char* component) {
  log(Severity::info, message, component);
}

void airmap::Logger::error(const char* message, const char* component) {
  log(Severity::error, message, component);
}

std::shared_ptr<airmap::Logger> airmap::create_default_logger(std::ostream& out) {
  return std::make_shared<DefaultLogger>(out);
}