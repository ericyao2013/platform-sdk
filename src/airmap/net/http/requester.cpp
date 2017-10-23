#include <airmap/net/http/requester.h>

airmap::net::http::RoutingRequester::RoutingRequester(const std::string& route, const std::shared_ptr<Requester>& next)
    : route_{route}, next_{next} {
}

void airmap::net::http::RoutingRequester::delete_(const std::string& path,
                                                  std::unordered_map<std::string, std::string>&& query,
                                                  std::unordered_map<std::string, std::string>&& headers, Callback cb) {
  next_->delete_(route_ + path, std::move(query), std::move(headers), std::move(cb));
}

void airmap::net::http::RoutingRequester::get(const std::string& path,
                                              std::unordered_map<std::string, std::string>&& query,
                                              std::unordered_map<std::string, std::string>&& headers, Callback cb) {
  next_->get(route_ + path, std::move(query), std::move(headers), std::move(cb));
}

void airmap::net::http::RoutingRequester::patch(const std::string& path,
                                                std::unordered_map<std::string, std::string>&& headers,
                                                const std::string& body, Callback cb) {
  next_->patch(route_ + path, std::move(headers), body, std::move(cb));
}

void airmap::net::http::RoutingRequester::post(const std::string& path,
                                               std::unordered_map<std::string, std::string>&& headers,
                                               const std::string& body, Callback cb) {
  next_->post(route_ + path, std::move(headers), body, std::move(cb));
}
