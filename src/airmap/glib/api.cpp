#include <airmap/glib/api.h>

#include <gio/gunixinputstream.h>
#include <glib.h>

#include <array>
#include <iostream>
#include <system_error>

namespace {

std::array<int, 2> create_pipe_or_throw() {
  std::array<int, 2> result;
  if (::pipe(&result[0]) == -1) throw std::system_error{errno, std::system_category()};
  return result;
}

}  // namespace

void airmap::glib::Api::create(const std::string& api_key, const CreateCallback& cb) {
  cb(CreateResult{std::shared_ptr<Api>{new Api{api_key}}});
}

void airmap::glib::Api::get(const std::string& host, const std::string& path,
                            std::unordered_map<std::string, std::string>&& query,
                            std::unordered_map<std::string, std::string>&& headers, DoCallback cb) {
  auto sp = shared_from_this();
  std::weak_ptr<Api> wp{sp};

  headers["X-API-Key"] = api_key_;

  auto uri = soup_uri_new(host.c_str());
  soup_uri_set_path(uri, path.c_str());

  GHashTable* query_table = g_hash_table_new(g_str_hash, g_str_equal);
  for (const auto& pair : query) {
    std::string key = pair.first;
    std::string value = pair.second;
    g_hash_table_insert(query_table, reinterpret_cast<gpointer>(g_strdup(&key.front())),
                        reinterpret_cast<gpointer>(g_strdup(&value.front())));
  }

  soup_uri_set_query_from_form(uri, query_table);
  auto msg = soup_message_new("GET", soup_uri_to_string(uri, FALSE));

  for (const auto& pair : headers)
    soup_message_headers_append(msg->request_headers, pair.first.c_str(), pair.second.c_str());

  g_hash_table_unref(query_table);
  soup_uri_free(uri);

  dispatch([this, wp, cb, msg]() {
    if (auto sp = wp.lock())
      soup_session_queue_message(session_.get(), msg, Api::soup_session_callback,
                                 new SoupSessionCallbackContext{cb, wp});
  });
}

void airmap::glib::Api::post(const std::string& host, const std::string& path,
                             std::unordered_map<std::string, std::string>&& headers, const std::string& body,
                             DoCallback cb) {
  auto sp = shared_from_this();
  std::weak_ptr<Api> wp{sp};

  headers["X-API-Key"] = api_key_;

  auto uri = soup_uri_new(host.c_str());
  soup_uri_set_path(uri, path.c_str());

  auto msg = soup_message_new("POST", soup_uri_to_string(uri, FALSE));
  for (const auto& pair : headers)
    soup_message_headers_append(msg->request_headers, pair.first.c_str(), pair.second.c_str());
  soup_message_set_request(msg, "application/json", SOUP_MEMORY_COPY, body.c_str(), body.size());

  soup_uri_free(uri);

  dispatch([this, wp, cb, msg]() {
    if (auto sp = wp.lock())
      soup_session_queue_message(session_.get(), msg, Api::soup_session_callback,
                                 new SoupSessionCallbackContext{cb, wp});
  });
}

void airmap::glib::Api::send_udp(const std::string& host, std::uint16_t port, const std::string& body) {
  dispatch([host, port, body = std::move(body)]() {
    if (auto connectable = g_network_address_parse(host.c_str(), port, nullptr)) {
      if (auto enumerator = g_socket_connectable_enumerate(connectable)) {
        if (auto address = g_socket_address_enumerator_next(enumerator, nullptr, nullptr)) {
          std::shared_ptr<GSocketAddress> sa{address, [](GSocketAddress* address) {
                                               if (address) g_object_unref(address);
                                             }};
          std::shared_ptr<GSocket> s{
              g_socket_new(G_SOCKET_FAMILY_IPV4, G_SOCKET_TYPE_DATAGRAM, G_SOCKET_PROTOCOL_UDP, nullptr),
              [](GSocket* socket) {
                if (socket) g_object_unref(socket);
              }};

          // TODO(tvoss): Add error handling here.
          g_socket_send_to(s.get(), sa.get(), body.c_str(), body.size(), nullptr, nullptr);
        }
        g_object_unref(enumerator);
      }
      g_object_unref(connectable);
    }
  });
}

void airmap::glib::Api::soup_session_callback(SoupSession*, SoupMessage* msg, gpointer user_data) {
  if (auto context = static_cast<SoupSessionCallbackContext*>(user_data)) {
    if (auto sp = context->wp.lock()) {
      auto cb = std::move(context->cb);
      delete (context);

      if (msg->response_body) {
        cb(DoResult{std::string{msg->response_body->data, static_cast<std::size_t>(msg->response_body->length)}});
      } else {
        cb(DoResult{msg->status_code});
      }
    }
  }
}

void airmap::glib::Api::on_pipe_fd_read_finished(GObject*, GAsyncResult*, gpointer user_data) {
  if (auto context = static_cast<Api*>(user_data)) {
    context->on_pipe_fd_read_finished();
  }
}

airmap::glib::Api::Api(const std::string& api_key)
    : api_key_{api_key},
      main_context_{g_main_context_new(), [](GMainContext* context) { g_main_context_unref(context); }},
      main_loop_{g_main_loop_new(main_context_.get(), FALSE), [](GMainLoop* ml) { g_main_loop_unref(ml); }},
      pipe_fds_{create_pipe_or_throw()},
      pipe_input_stream_{g_unix_input_stream_new(pipe_fds_[0], FALSE),
                         [](GInputStream* stream) {
                           if (stream) g_object_unref(G_OBJECT(stream));
                         }},
      session_{soup_session_new(),
               [](SoupSession* session) {
                 if (session) g_object_unref(session);
               }} {
  worker_ = std::thread{[this]() {
    g_main_context_push_thread_default(main_context_.get());
    g_input_stream_read_async(pipe_input_stream_.get(), &pipe_read_buffer_, sizeof(pipe_read_buffer_), G_PRIORITY_LOW,
                              nullptr, Api::on_pipe_fd_read_finished, this);
    g_main_loop_run(main_loop_.get());
    g_main_context_pop_thread_default(main_context_.get());
  }};
}

airmap::glib::Api::~Api() {
  g_main_loop_quit(main_loop_.get());
  if (worker_.joinable()) {
    worker_.join();
  }
}

void airmap::glib::Api::dispatch(const std::function<void()>& task) {
  static const int value{42};

  std::lock_guard<std::mutex> lg{guard_};
  functors_.push(task);
  ::write(pipe_fds_[1], &value, sizeof(value));
}

void airmap::glib::Api::on_pipe_fd_read_finished() {
  std::queue<std::function<void()>> functors;

  {
    std::lock_guard<std::mutex> lg{guard_};
    std::swap(functors_, functors);
  }

  while (!functors.empty()) {
    functors.front()();
    functors.pop();
  }

  g_input_stream_read_async(pipe_input_stream_.get(), &pipe_read_buffer_, sizeof(pipe_read_buffer_), G_PRIORITY_LOW,
                            nullptr, Api::on_pipe_fd_read_finished, this);
}
