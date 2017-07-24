#ifndef AIRMAP_REST_COMMUNICATOR_H_
#define AIRMAP_REST_COMMUNICATOR_H_

#include <airmap/optional.h>
#include <airmap/outcome.h>

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

class Communicator : public std::enable_shared_from_this<Communicator> {
 public:
  using Status = std::uint16_t;

  using CreateResult   = Outcome<std::shared_ptr<Communicator>, std::exception_ptr>;
  using CreateCallback = std::function<void(const CreateResult&)>;
  using DoResult       = Outcome<std::string, Status>;
  using DoCallback     = std::function<void(const DoResult&)>;

  static void create(const std::string& api_key, const CreateCallback& cb);

  ~Communicator();

  void get(const std::string& host, const std::string& path,
           std::unordered_map<std::string, std::string>&& query,
           std::unordered_map<std::string, std::string>&& headers, DoCallback cb);
  void post(const std::string& host, const std::string& path,
            std::unordered_map<std::string, std::string>&& headers, const std::string& body,
            DoCallback cb);
  void send_udp(const std::string& host, std::uint16_t port, const std::string& body);

 private:
  struct SoupSessionCallbackContext {
    DoCallback cb;
    std::weak_ptr<Communicator> wp;
  };

  static void soup_session_callback(SoupSession* session, SoupMessage* msg, gpointer user_data);
  static void on_pipe_fd_read_finished(GObject* source, GAsyncResult* result, gpointer user_data);

  explicit Communicator(const std::string& api_key);

  void dispatch(const std::function<void()>& task);
  void on_pipe_fd_read_finished();

  std::string api_key_;

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

}  // namespace rest
}  // namespace airmap

#endif  // AIRMAP_REST_COMMUNICATOR_H_
