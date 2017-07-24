#include <airmap/rest/airspaces.h>

#include <airmap/codec.h>
#include <airmap/jsend.h>

#include <boost/format.hpp>

#include <sstream>

using json = nlohmann::json;

airmap::rest::Airspaces::Airspaces(airmap::rest::Communicator& communicator)
    : communicator_{communicator} {
}

void airmap::rest::Airspaces::search(const Search::Parameters& parameters,
                                     const Search::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  communicator_.get("https://api.airmap.com", "/airspace/v2/search", std::move(query),
                    std::move(headers), [cb](const Communicator::DoResult& result) {
                      if (result) {
                        auto j = json::parse(result.value());
                        if (j[jsend::key::status] == jsend::status::success) {
                          cb(Search::Result(j[jsend::key::data].get<std::vector<Airspace>>()));
                        } else {
                          cb(Search::Result(std::make_exception_ptr(
                              std::runtime_error(jsend::stringify_error(j)))));
                        }
                      }
                    });
}

void airmap::rest::Airspaces::for_ids(const ForIds::Parameters& parameters,
                                      const ForIds::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  communicator_.get(
      "https://api.airmap.com", (boost::format("/airspace/v2/%1%") % parameters.id).str(),
      std::move(query), std::move(headers), [cb](const Communicator::DoResult& result) {
        if (result) {
          auto j = json::parse(result.value());

          if (j[jsend::key::status] == jsend::status::success) {
            cb(ForIds::Result(j[jsend::key::data].get<std::vector<Airspace>>()));
          } else {
            cb(ForIds::Result(
                std::make_exception_ptr(std::runtime_error(jsend::stringify_error(j)))));
          }
        }
      });
}
