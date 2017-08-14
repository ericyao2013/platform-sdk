#ifndef AIRMAP_PILOTS_H_
#define AIRMAP_PILOTS_H_

#include <airmap/aircraft.h>
#include <airmap/do_not_copy_or_move.h>
#include <airmap/optional.h>
#include <airmap/outcome.h>
#include <airmap/pilot.h>

#include <functional>
#include <iosfwd>
#include <string>
#include <vector>

namespace airmap {

class Pilots : DoNotCopyOrMove {
 public:
  enum class Exclude { aircraft = 1 << 0, user_metadata = 1 << 1, app_metadata = 1 << 2, authorized_apps = 1 << 3 };

  struct Authenticated {
    struct Parameters {
      std::string authorization;
      Optional<Exclude> exclude{};
      bool retrieve_statistics{false};
    };

    using Result   = Outcome<Pilot, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  struct ForId {
    struct Parameters {
      std::string authorization;
      std::string id;
      Optional<Exclude> exclude{};
      bool retrieve_statistics{false};
    };

    using Result   = Outcome<Pilot, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  struct UpdateForId {
    struct Parameters {
      std::string authorization;
      std::string id;
      std::string first_name;
      std::string last_name;
      std::string user_name;
      std::string phone;
      std::map<std::string, std::string> app_metadata;
      std::map<std::string, std::string> user_metadata;
    };

    using Result   = Outcome<Pilot, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  struct StartVerifyPilotPhoneForId {
    struct Parameters {
      std::string authorization;
      std::string id;
    };

    struct Empty {};

    using Result   = Outcome<Empty, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  struct FinishVerifyPilotPhoneForId {
    struct Parameters {
      std::string authorization;
      std::string id;
      std::uint32_t token;
    };

    struct Empty {};

    using Result   = Outcome<Empty, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  struct Aircrafts {
    struct Parameters {
      std::string authorization;
      std::string id;
    };

    using Result   = Outcome<std::vector<Pilot::Aircraft>, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  struct AddAircraft {
    struct Parameters {
      std::string authorization;
      std::string id;
      std::string model_id;
      std::string nick_name;
    };

    using Result   = Outcome<Pilot::Aircraft, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  struct DeleteAircraft {
    struct Parameters {
      std::string authorization;
      std::string id;
      std::string aircraft_id;
    };

    struct Empty {};

    using Result   = Outcome<Empty, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  struct UpdateAircraft {
    struct Parameters {
      std::string authorization;
      std::string id;
      std::string aircraft_id;
      std::string nick_name;
    };

    struct Empty {};

    using Result   = Outcome<Empty, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  /// current_user queries the AirMap services for the pilot profile
  /// connected to the authenticated user, reporting results to 'cb'.
  virtual void authenticated(const Authenticated::Parameters& parameters, const Authenticated::Callback& cb) = 0;

  /// for_id queries the AirMap services for the pilot profile
  /// with a given id, reporting results to 'cb'.
  virtual void for_id(const ForId::Parameters& parameters, const ForId::Callback& cb) = 0;

  /// update_for_id updates the pilot profile specified
  // by Parameters::id, reporting results to 'cb'.
  virtual void update_for_id(const UpdateForId::Parameters& parameters, const UpdateForId::Callback& cb) = 0;

  /// start_verify_pilot_phone_for_id sends a verification token to the phone
  /// number stored in the pilot profile, reporting results to 'cb'.
  virtual void start_verify_pilot_phone_for_id(const StartVerifyPilotPhoneForId::Parameters& parameters,
                                               const StartVerifyPilotPhoneForId::Callback& cb) = 0;

  /// finish_verify_pilot_phone_for_id responds to a verification request by
  /// sending back the token sent to the pilot's phone, reporting results to 'cb'.
  virtual void finish_verify_pilot_phone_for_id(const FinishVerifyPilotPhoneForId::Parameters& parameters,
                                                const FinishVerifyPilotPhoneForId::Callback& cb) = 0;

  /// aircrafts queries the list of aircrafts owned by a pilot, reporting results to 'cb'.
  virtual void aircrafts(const Aircrafts::Parameters& parameters, const Aircrafts::Callback& cb) = 0;

  /// add_aircraft associates a new aircraft with a pilot, reporting results to 'cb'.
  virtual void add_aircraft(const AddAircraft::Parameters& parameters, const AddAircraft::Callback& cb) = 0;

  /// delete_aircraft removes an aircraft from a pilot profile, reporting results to 'cb'.
  virtual void delete_aircraft(const DeleteAircraft::Parameters& parameters, const DeleteAircraft::Callback& cb) = 0;

  /// update_aircraft updates the properties of an aircraft associated with a pilot, reporting results to 'cb'.
  virtual void update_aircraft(const UpdateAircraft::Parameters& parameters, const UpdateAircraft::Callback& cb) = 0;

 protected:
  Pilots() = default;
};

Pilots::Exclude operator|(Pilots::Exclude, Pilots::Exclude);
Pilots::Exclude operator&(Pilots::Exclude, Pilots::Exclude);
std::ostream& operator<<(std::ostream& out, Pilots::Exclude exclude);

}  // namespace airmap

#endif  // AIRMAP_PILOTS_H_
