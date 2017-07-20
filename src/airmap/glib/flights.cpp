#include <airmap/glib/flights.h>

#include <airmap/codec.h>
#include <airmap/jsend.h>

#include <boost/format.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

airmap::glib::Flights::Flights(Api& api) : api_{api} {
}

void airmap::glib::Flights::search(const Search::Parameters& parameters, const Search::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;

  if (parameters.authorization)
    headers["Authorization"] = (boost::format("Bearer %1%") % parameters.authorization.get()).str();
  codec::http::query::encode(query, parameters);

  api_.get("https://api.airmap.com", "/flight/v2/", std::move(query), std::move(headers),
           [cb](const Api::DoResult& result) {
             if (result) {
               auto j = json::parse(result.value());
               if (j[jsend::key::status] == jsend::status::success) {
                 cb(Search::Result(j[jsend::key::data]["results"].get<std::vector<Flight>>()));
               } else {
                 cb(Search::Result(std::make_exception_ptr(std::runtime_error(jsend::stringify_error(j)))));
               }
             }
           });
}

void airmap::glib::Flights::for_id(const ForId::Parameters& parameters, const ForId::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;

  if (parameters.authorization)
    headers["Authorization"] = (boost::format("Bearer %1%") % parameters.authorization.get()).str();
  codec::http::query::encode(query, parameters);

  api_.get("https://api.airmap.com", (boost::format("/flight/v2/%1%") % parameters.id).str(), std::move(query),
           std::move(headers), [cb](const Api::DoResult& result) {
             if (result) {
               auto j = json::parse(result.value());
               if (j[jsend::key::status] == jsend::status::success) {
                 cb(ForId::Result(j[jsend::key::data].get<Flight>()));
               } else {
                 cb(ForId::Result(std::make_exception_ptr(std::runtime_error(jsend::stringify_error(j)))));
               }
             }
           });
}

void airmap::glib::Flights::create_flight_by_point(const CreateFlight::Parameters& parameters,
                                                   const CreateFlight::Callback& cb) {
  std::unordered_map<std::string, std::string> headers{
      {"Authorization", (boost::format("Bearer %1%") % parameters.authorization).str()}};

  json j;
  j = parameters;

  api_.post("https://api.airmap.com", "/flight/v2/point", std::move(headers), j.dump(),
            [cb](const Api::DoResult& result) {
              if (result) {
                auto j = json::parse(result.value());
                std::cout << j.dump(2) << std::endl;
                if (j[jsend::key::status] == jsend::status::success) {
                  cb(CreateFlight::Result(j[jsend::key::data].get<Flight>()));
                } else {
                  cb(CreateFlight::Result(std::make_exception_ptr(std::runtime_error(jsend::stringify_error(j)))));
                }
              }
            });
}

void airmap::glib::Flights::create_flight_by_path(const CreateFlight::Parameters& parameters,
                                                  const CreateFlight::Callback& cb) {
  std::unordered_map<std::string, std::string> headers{
      {"Authorization", (boost::format("Bearer %1%") % parameters.authorization).str()}};

  json j;
  j = parameters;

  api_.post("https://api.airmap.com", "/flight/v2/path", std::move(headers), j.dump(),
            [cb](const Api::DoResult& result) {
              if (result) {
                auto j = json::parse(result.value());
                if (j[jsend::key::status] == jsend::status::success) {
                  cb(CreateFlight::Result(j[jsend::key::data].get<Flight>()));
                } else {
                  cb(CreateFlight::Result(std::make_exception_ptr(std::runtime_error(jsend::stringify_error(j)))));
                }
              }
            });
}

void airmap::glib::Flights::create_flight_by_polygon(const CreateFlight::Parameters& parameters,
                                                     const CreateFlight::Callback& cb) {
  std::unordered_map<std::string, std::string> headers{
      {"Authorization", (boost::format("Bearer %1%") % parameters.authorization).str()}};

  json j;
  j = parameters;

  api_.post("https://api.airmap.com", "/flight/v2/polygon", std::move(headers), j.dump(),
            [cb](const Api::DoResult& result) {
              if (result) {
                auto j = json::parse(result.value());
                if (j[jsend::key::status] == jsend::status::success) {
                  cb(CreateFlight::Result(j[jsend::key::data].get<Flight>()));
                } else {
                  cb(CreateFlight::Result(std::make_exception_ptr(std::runtime_error(jsend::stringify_error(j)))));
                }
              }
            });
}

void airmap::glib::Flights::end_flight(const EndFlight::Parameters& parameters, const EndFlight::Callback& cb) {
  std::unordered_map<std::string, std::string> headers{
      {"Authorization", (boost::format("Bearer %1%") % parameters.authorization).str()}};

  api_.post("https://api.airmap.com", (boost::format("/flight/v2/%1%/end") % parameters.id).str(), std::move(headers),
            std::string{}, [cb](const Api::DoResult& result) {
              if (result) {
                auto j = json::parse(result.value());
                if (j[jsend::key::status] == jsend::status::success) {
                  cb(EndFlight::Result(j[jsend::key::data].get<EndFlight::Response>()));
                } else {
                  cb(EndFlight::Result(std::make_exception_ptr(std::runtime_error(jsend::stringify_error(j)))));
                }
              }
            });
}

void airmap::glib::Flights::delete_flight(const DeleteFlight::Parameters& parameters,
                                          const DeleteFlight::Callback& cb) {
  std::unordered_map<std::string, std::string> headers{
      {"Authorization", (boost::format("Bearer %1%") % parameters.authorization).str()}};

  api_.post("https://api.airmap.com", (boost::format("/flight/v2/%1%/delete") % parameters.id).str(), std::move(headers),
            std::string{}, [cb](const Api::DoResult& result) {
              if (result) {
                auto j = json::parse(result.value());
                if (j[jsend::key::status] == jsend::status::success) {
                  cb(DeleteFlight::Result(j[jsend::key::data].get<DeleteFlight::Response>()));
                } else {
                  cb(DeleteFlight::Result(std::make_exception_ptr(std::runtime_error(jsend::stringify_error(j)))));
                }
              }
            });
}

void airmap::glib::Flights::start_flight_communications(const StartFlightCommunications::Parameters& parameters,
                                                        const StartFlightCommunications::Callback& cb) {
  std::unordered_map<std::string, std::string> headers{
      {"Authorization", (boost::format("Bearer %1%") % parameters.authorization).str()}};

  api_.post("https://api.airmap.com", (boost::format("/flight/v2/%1%/start-comm") % parameters.id).str(), std::move(headers),
            std::string{}, [cb](const Api::DoResult& result) {
              if (result) {
                auto j = json::parse(result.value());
                if (j[jsend::key::status] == jsend::status::success) {
                  cb(StartFlightCommunications::Result(j[jsend::key::data].get<StartFlightCommunications::Response>()));
                } else {
                  cb(StartFlightCommunications::Result(std::make_exception_ptr(std::runtime_error(jsend::stringify_error(j)))));
                }
              }
            });                                                          
}

void airmap::glib::Flights::end_flight_communications(const EndFlightCommunications::Parameters& parameters,
                                                      const EndFlightCommunications::Callback& cb) {
  std::unordered_map<std::string, std::string> headers{
      {"Authorization", (boost::format("Bearer %1%") % parameters.authorization).str()}};

  api_.post("https://api.airmap.com", (boost::format("/flight/v2/%1%/end-comm") % parameters.id).str(), std::move(headers),
            std::string{}, [cb](const Api::DoResult& result) {
              if (result) {
                auto j = json::parse(result.value());
                if (j[jsend::key::status] == jsend::status::success) {
                  cb(EndFlightCommunications::Result(j[jsend::key::data].get<EndFlightCommunications::Response>()));
                } else {
                  cb(EndFlightCommunications::Result(std::make_exception_ptr(std::runtime_error(jsend::stringify_error(j)))));
                }
              }
            });
}