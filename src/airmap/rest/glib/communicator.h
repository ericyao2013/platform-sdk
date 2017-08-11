#ifndef AIRMAP_REST_GLIB_COMMUNICATOR_H_
#define AIRMAP_REST_GLIB_COMMUNICATOR_H_

#include <airmap/context.h>
#include <airmap/rest/communicator.h>
#include <airmap/util/formatting_logger.h>

#include <glib.h>
#include <libsoup/soup.h>

#include <array>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

namespace airmap {
namespace rest {
namespace glib {

class Communicator : public airmap::rest::Communicator,
                     public airmap::Context,
                     public std::enable_shared_from_this<Communicator> {
 public:
  Communicator(const std::shared_ptr<Logger>& logger);
  ~Communicator();

  // From airmap::Context
  void create_client_with_configuration(const Client::Configuration& configuration,
                                        const ClientCreateCallback& cb) override;
  void run() override;
  void stop() override;

  // From airmap::rest::Communicator
  void get(const std::string& host, const std::string& path, std::unordered_map<std::string, std::string>&& query,
           std::unordered_map<std::string, std::string>&& headers, DoCallback cb) override;
  void post(const std::string& host, const std::string& path, std::unordered_map<std::string, std::string>&& headers,
            const std::string& body, DoCallback cb) override;
  void send_udp(const std::string& host, std::uint16_t port, const std::string& body) override;
  void dispatch(const std::function<void()>& task) override;

 private:
  struct SoupSessionCallbackContext {
    DoCallback cb;
    std::weak_ptr<Communicator> wp;
  };

  static void soup_session_callback(SoupSession* session, SoupMessage* msg, gpointer user_data);
  static void on_pipe_fd_read_finished(GObject* source, GAsyncResult* result, gpointer user_data);

  void on_pipe_fd_read_finished();

  util::FormattingLogger log_;

  std::shared_ptr<GMainContext> main_context_;
  std::shared_ptr<GMainLoop> main_loop_;

  std::array<int, 2> pipe_fds_;
  int pipe_read_buffer_;
  std::shared_ptr<GInputStream> pipe_input_stream_;
  std::mutex guard_;
  std::queue<std::function<void()>> functors_;

  std::shared_ptr<SoupSession> session_;
  std::thread worker_;
};

}  // namespace glib
}  // namespace rest
}  // namespace airmap

#endif  // AIRMAP_REST_GLIB_COMMUNICATOR_H_
