#ifndef AIRMAP_REST_PILOTS_H_
#define AIRMAP_REST_PILOTS_H_

#include <airmap/client.h>
#include <airmap/pilots.h>

#include <airmap/rest/communicator.h>

namespace airmap {
namespace rest {

class Pilots : public airmap::Pilots {
 public:
  explicit Pilots(const std::string& host, Client::Version version, Communicator& communicator);

  void authenticated(const Authenticated::Parameters& parameters, const Authenticated::Callback& cb) override;
  void for_id(const ForId::Parameters& parameters, const ForId::Callback& cb) override;
  void update_for_id(const UpdateForId::Parameters& parameters, const UpdateForId::Callback& cb) override;
  void start_verify_pilot_phone_for_id(const StartVerifyPilotPhoneForId::Parameters& parameters,
                                       const StartVerifyPilotPhoneForId::Callback& cb) override;
  void finish_verify_pilot_phone_for_id(const FinishVerifyPilotPhoneForId::Parameters& parameters,
                                        const FinishVerifyPilotPhoneForId::Callback& cb) override;
  void aircrafts(const Aircrafts::Parameters& parameters, const Aircrafts::Callback& cb) override;
  void add_aircraft(const AddAircraft::Parameters& parameters, const AddAircraft::Callback& cb) override;
  void delete_aircraft(const DeleteAircraft::Parameters& parameters, const DeleteAircraft::Callback& cb) override;
  void update_aircraft(const UpdateAircraft::Parameters& parameters, const UpdateAircraft::Callback& cb) override;

 private:
  std::string host_;
  Client::Version version_;
  Communicator& communicator_;
};

}  // namespace rest
}  // namespace airmap

#endif  // AIRMAP_REST_PILOTS_H_
