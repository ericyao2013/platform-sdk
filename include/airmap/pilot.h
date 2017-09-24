#ifndef AIRMAP_PILOT_H_
#define AIRMAP_PILOT_H_

#include <airmap/aircraft.h>
#include <airmap/date_time.h>
#include <airmap/optional.h>

#include <cstdint>

#include <map>
#include <string>

namespace airmap {

/// Pilot bundles up all properties describing a pilot on the AirMap services.
struct Pilot {
  /// Aircraft describes a vehicle owned by a Pilot.
  struct Aircraft {
    std::string id;
    std::string nick_name;
    airmap::Aircraft model;
    DateTime created_at;
  };

  std::string id;                     /// The unique id of the pilot in the context of AirMap.
  std::string first_name;             /// The first name of the pilot.
  std::string last_name;              /// The last name of the pilot.
  std::string user_name;              /// The AirMap username of this pilot.
  Optional<std::string> picture_url;  /// The URL of a picture showing the pilot.

  /// VerificationStatus summarizes the
  /// status of contact detail verification.
  struct VerificationStatus {
    bool email;  ///< true iff the email address of the pilot has been verified
    bool phone;  ///< true iff the phone number of the pilot has been verified
  } verification_status;

  /// Statistics about the pilot and her
  /// flight experience as recorded by the
  /// AirMap services.
  struct Statistics {
    struct Flight {
      std::uint64_t total;        ///< The total number of flights
      DateTime last_flight_time;  ///< Date and time of the last flight
    } flight;
    struct Aircraft {
      std::uint64_t total;  ///< The total number of aircrafts
    } aircraft;
  } statistics;

  /// App- and user-specific metadata.
  struct Metadata {
    std::map<std::string, std::string> app;
    std::map<std::string, std::string> user;
  } metadata;
  DateTime created_at;  /// Timestamp of the creation of this pilot in the AirMap system.
};

}  // namespace airmap

#endif  // AIRMAP_PILOT_H_
