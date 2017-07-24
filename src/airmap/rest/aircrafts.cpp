#include <airmap/rest/aircrafts.h>

#include <airmap/codec.h>
#include <airmap/jsend.h>

#include <boost/format.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

airmap::rest::Aircrafts::Aircrafts(Communicator& communicator) : communicator_{communicator} {
}

void airmap::rest::Aircrafts::manufacturers(const Manufacturers::Parameters& parameters,
                                            const Manufacturers::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  communicator_.get("https://api.airmap.com", "/aircraft/v2/manufacturer", std::move(query),
                    std::move(headers), [cb](const Communicator::DoResult& result) {
                      if (result) {
                        auto j = json::parse(result.value());

                        if (j[jsend::key::status] == jsend::status::success) {
                          cb(Manufacturers::Result(
                              j[jsend::key::data].get<std::vector<Aircraft::Manufacturer>>()));
                        } else if (j[jsend::key::status] == jsend::status::error) {
                          cb(Manufacturers::Result(std::make_exception_ptr(
                              std::runtime_error(j[jsend::key::message].get<std::string>()))));
                        } else if (j[jsend::key::status] == jsend::status::failure) {
                          cb(Manufacturers::Result(std::make_exception_ptr(
                              std::runtime_error(j[jsend::key::message].get<std::string>()))));
                        } else {
                          cb(Manufacturers::Result(std::make_exception_ptr(
                              std::runtime_error(j[jsend::key::message].get<std::string>()))));
                        }
                      }
                    });
}

void airmap::rest::Aircrafts::models(const Models::Parameters& parameters,
                                     const Models::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  communicator_.get("https://api.airmap.com", "/aircraft/v2/model", std::move(query),
                    std::move(headers), [cb](const Communicator::DoResult& result) {
                      if (result) {
                        auto j = json::parse(result.value());

                        if (j[jsend::key::status] == jsend::status::success) {
                          cb(Models::Result(j[jsend::key::data].get<std::vector<Aircraft>>()));
                        } else {
                          cb(Models::Result(std::make_exception_ptr(
                              std::runtime_error(jsend::stringify_error(j)))));
                        }
                      }
                    });
}

void airmap::rest::Aircrafts::model_for_id(const ModelForId::Parameters& parameters,
                                           const ModelForId::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;

  communicator_.get(
      "https://api.airmap.com", (boost::format("/aircraft/v2/model/%1%") % parameters.id).str(),
      std::move(query), std::move(headers), [cb](const Communicator::DoResult& result) {
        if (result) {
          auto j = json::parse(result.value());

          if (j[jsend::key::status] == jsend::status::success) {
            cb(ModelForId::Result(j[jsend::key::data].get<Aircraft>()));
          } else {
            cb(ModelForId::Result(
                std::make_exception_ptr(std::runtime_error(jsend::stringify_error(j)))));
          }
        }
      });
}