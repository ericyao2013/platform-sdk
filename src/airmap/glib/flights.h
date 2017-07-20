#ifndef AIRMAP_GLIB_FLIGHTS_H_
#define AIRMAP_GLIB_FLIGHTS_H_

#include <airmap/flights.h>

#include <airmap/glib/api.h>

namespace airmap {
namespace glib {

class Flights : public airmap::Flights {
 public:
  explicit Flights(Api& api);

  void search(const Search::Parameters& parameters, const Search::Callback& cb) override;
  void for_id(const ForId::Parameters& parameters, const ForId::Callback& cb) override;
  void create_flight_by_point(const CreateFlight::Parameters& parameters,
                              const CreateFlight::Callback& cb) override;
  void create_flight_by_path(const CreateFlight::Parameters& parameters,
                             const CreateFlight::Callback& cb) override;
  void create_flight_by_polygon(const CreateFlight::Parameters& parameters,
                                const CreateFlight::Callback& cb) override;
  void end_flight(const EndFlight::Parameters& parameters, const EndFlight::Callback& cb) override;
  void delete_flight(const DeleteFlight::Parameters& parameters,
                     const DeleteFlight::Callback& cb) override;
  void start_flight_communications(const StartFlightCommunications::Parameters& parameters,
                                   const StartFlightCommunications::Callback& cb) override;
  virtual void end_flight_communications(const EndFlightCommunications::Parameters& parameters,
                                         const EndFlightCommunications::Callback& cb) override;

 private:
  Api& api_;
};

}  // namespace glib
}  // namespace airmap

#endif  // AIRMAP_GLIB_FLIGHTS_H_
