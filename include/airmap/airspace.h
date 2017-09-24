#ifndef AIRMAP_AIRSPACE_H_
#define AIRMAP_AIRSPACE_H_

#include <airmap/geometry.h>
#include <airmap/optional.h>
#include <airmap/rule.h>
#include <airmap/timestamp.h>

#include <cstdint>
#include <iosfwd>
#include <string>
#include <vector>

namespace airmap {

/// Airport bundles up properties further describing an
/// airspace around an airport.
struct Airport {
  /// Runway describes an individual runway of an airport.
  struct Runway {
    std::string name;
    float length;
    float bearing;
  };

  /// Use enumerates all known usage types for
  /// an airport.
  enum class Use { public_ };

  std::string iata;
  std::string icao;
  bool paved;
  std::string phone;
  bool tower;
  std::vector<Runway> runways;
  float elevation;
  float longest_runway;
  bool instrument_approach_procedure;
  Use use;
};

/// ControlledAirspace bundles up properties describing
/// a controlled airspace.
struct ControlledAirspace {};

/// SpecialUseAirspace bundles up properties describing
/// a special use airspace.
struct SpecialUseAirspace {
  enum class Type {};
  Type type;
};

/// TemporaryFlightRestriction describes an airspace that
/// modelling a temporary restriction of the airspace.
struct TemporaryFlightRestriction {
  enum class Type {};
  std::string url;
  Type type;
  std::string reason;
};

/// Wildfire describes an airspace around a wildfire.
struct Wildfire {
  std::string effective_date;
};

struct Park {};

struct Prison {};

struct School {};

struct Hospital {};

struct Fire {};

struct Emergency {};

/// Heliport describes an airspace around a heliport.
struct Heliport {
  enum class Usage {};
  std::string faa_id;
  std::string phone;
  Usage usage;
};

/// Powerplant describes the airspace around a powerplant.
struct PowerPlant {
  std::string technology;
  std::uint64_t code;
};

// Compactly places general and detailed
// data within one object. The Airspace description
// has hardly any behavior on its own. More to this,
// relying on a compact representation instead of modelling
// an inheritance hierarchy helps on "embedded" platforms in
// passing around instances.
class Airspace {
 public:
  // Enumerates all known airspace types.
  enum class Type {
    invalid              = 0,
    airport              = 1 << 0,
    controlled_airspace  = 1 << 1,
    special_use_airspace = 1 << 2,
    tfr                  = 1 << 3,
    wildfire             = 1 << 4,
    park                 = 1 << 5,
    power_plant          = 1 << 6,
    heliport             = 1 << 7,
    prison               = 1 << 8,
    school               = 1 << 9,
    hospital             = 1 << 10,
    fire                 = 1 << 11,
    emergency            = 1 << 12,
    all = airport | controlled_airspace | special_use_airspace | tfr | wildfire | park | power_plant | heliport |
          prison | school | hospital | fire | emergency
  };

  using Id = std::string;

  /// @cond
  Airspace();
  Airspace(const Airspace &rhs);
  ~Airspace();
  Airspace &operator=(const Airspace &rhs);
  bool operator==(const Airspace &rhs) const;
  bool operator!=(const Airspace &rhs) const;
  /// @endcond

  /// id returns an immutable reference to the unique idd
  /// of this airspace.
  const Id &id() const;
  /// set_id adjusts the unique id of this airspace to id.
  void set_id(const Id &id);

  /// name returns an immutable reference to the
  /// human-readable name of this airspace.
  const std::string &name() const;
  /// set_name adjusts the name of this airspace to name.
  void set_name(const std::string &name);

  /// type returns the Type of this airspace instance.
  Type type() const;

  /// country returns an immutable reference to the name of the country
  /// that the airspace belongs to.
  const std::string &country() const;
  /// set_country adjusts the name of the country that this airspace instance belongs to.
  void set_country(const std::string &country);

  /// state returns an immutable reference to the name of the state
  /// that the airspace belongs to.
  const std::string &state() const;
  /// set_state adjusts the name of the state that this airspace instance belongs to.
  void set_state(const std::string &state);

  /// city returns an immutable reference to the name of the city
  /// that the airspace belongs to.
  const std::string &city() const;
  /// set_city adjusts the name of the city that this airspace instance belongs to.
  void set_city(const std::string &city);

  /// last_updated returns an immutable reference to the timestamp of the last update
  /// to this airspace instance.
  const Timestamp &last_updated() const;
  /// set_last_updated adjusts the timestamp of the last update to this airspace to
  /// 'timestamp'.
  void set_last_updated(const Timestamp &timestamp);

  /// geometry returns an immutable reference to the geometry of this airspace instance.
  const Geometry &geometry() const;
  /// set_geometry adjusts the geometry of this airspace instance to 'geometry'.
  void set_geometry(const Geometry &geometry);

  /// related_geometries returns an immutable reference to all geometries associated with
  /// this airspace instance.
  const std::vector<Geometry> &related_geometries() const;
  /// set_related_geometries adjusts the geometries associated with this airspace instance
  /// to 'geometry'.
  void set_related_geometries(const std::vector<Geometry> &geometries);

  /// rules returns an immutable reference to the rules applying to this airspace instance.
  const std::vector<Rule> &rules() const;
  /// set_rules adjusts the rules applying to this airspace instance to 'rules.
  void set_rules(const std::vector<Rule> &rules);

  /// details_for_airport returns an immutable reference to the details
  /// further describing this airspace instance.
  const Airport &details_for_airport() const;
  /// details_for_airport returns a mutable reference to the details
  /// further describing this airspace instance.
  Airport &details_for_airport();

  /// details_for_controlled_airspace returns an immutable reference to the details
  /// further describing this airspace instance.
  const ControlledAirspace &details_for_controlled_airspace() const;
  /// details_for_controlled_airspace returns a mutable reference to the details
  /// further describing this airspace instance.
  ControlledAirspace &details_for_controlled_airspace();

  /// details_for_emergency returns an immutable reference to the details
  /// further describing this airspace instance.
  const Emergency &details_for_emergency() const;
  /// details_for_emergency returns a mutable reference to the details
  /// further describing this airspace instance.
  Emergency &details_for_emergency();

  /// details_for_fire returns an immutable reference to the details
  /// further describing this airspace instance.
  const Fire &details_for_fire() const;
  /// details_for_fire returns a mutable reference to the details
  /// further describing this airspace instance.
  Fire &details_for_fire();

  /// details_for_heliport returns an immutable reference to the details
  /// further describing this airspace instance.
  const Heliport &details_for_heliport() const;
  /// details_for_heliport returns a mutable reference to the details
  /// further describing this airspace instance.
  Heliport &details_for_heliport();

  /// details_for_hospital returns an immutable reference to the details
  /// further describing this airspace instance.
  const Hospital &details_for_hospital() const;
  /// details_for_hospital returns a mutable reference to the details
  /// further describing this airspace instance.
  Hospital &details_for_hospital();

  /// details_for_park returns an immutable reference to the details
  /// further describing this airspace instance.
  const Park &details_for_park() const;
  /// details_for_park returns a mutable reference to the details
  /// further describing this airspace instance.
  Park &details_for_park();

  /// details_for_power_plant returns an immutable reference to the details
  /// further describing this airspace instance.
  const PowerPlant &details_for_power_plant() const;
  /// details_for_power_plant returns a mutable reference to the details
  /// further describing this airspace instance.
  PowerPlant &details_for_power_plant();

  /// details_for_prison returns an immutable reference to the details
  /// further describing this airspace instance.
  const Prison &details_for_prison() const;
  /// details_for_prison returns a mutable reference to the details
  /// further describing this airspace instance.
  Prison &details_for_prison();

  /// details_for_school returns an immutable reference to the details
  /// further describing this airspace instance.
  const School &details_for_school() const;
  /// details_for_school returns a mutable reference to the details
  /// further describing this airspace instance.
  School &details_for_school();

  /// details_for_special_use_airspace returns an immutable reference to the details
  /// further describing this airspace instance.
  const SpecialUseAirspace &details_for_special_use_airspace() const;
  /// details_for_special_use_airspace returns a mutable reference to the details
  /// further describing this airspace instance.
  SpecialUseAirspace &details_for_special_use_airspace();

  /// details_for_temporary_flight_restriction returns an immutable reference to the details
  /// further describing this airspace instance.
  const TemporaryFlightRestriction &details_for_temporary_flight_restriction() const;
  /// details_for_temporary_flight_restriction returns a mutable reference to the details
  /// further describing this airspace instance.
  TemporaryFlightRestriction &details_for_temporary_flight_restriction();

  /// details_for_wildfire returns an immutable reference to the details
  /// further describing this airspace instance.
  const Wildfire &details_for_wildfire() const;
  /// details_for_wildfire returns an immutable reference to the details
  /// further describing this airspace instance.
  Wildfire &details_for_wildfire();

  /// set_details adjusts the details of this airspace instance to 'detail'.
  void set_details(const Airspace &detail);
  /// set_details adjusts the details of this airspace instance to 'detail'.
  void set_details(const Airport &detail);
  /// set_details adjusts the details of this airspace instance to 'detail'.
  void set_details(const ControlledAirspace &detail);
  /// set_details adjusts the details of this airspace instance to 'detail'.
  void set_details(const SpecialUseAirspace &detail);
  /// set_details adjusts the details of this airspace instance to 'detail'.
  void set_details(const TemporaryFlightRestriction &detail);
  /// set_details adjusts the details of this airspace instance to 'detail'.
  void set_details(const Wildfire &detail);
  /// set_details adjusts the details of this airspace instance to 'detail'.
  void set_details(const Park &detail);
  /// set_details adjusts the details of this airspace instance to 'detail'.
  void set_details(const PowerPlant &detail);
  /// set_details adjusts the details of this airspace instance to 'detail'.
  void set_details(const Heliport &detail);
  /// set_details adjusts the details of this airspace instance to 'detail'.
  void set_details(const Prison &detail);
  /// set_details adjusts the details of this airspace instance to 'detail'.
  void set_details(const School &detail);
  /// set_details adjusts the details of this airspace instance to 'detail'.
  void set_details(const Hospital &detail);
  /// set_details adjusts the details of this airspace instance to 'detail'.
  void set_details(const Fire &detail);
  /// set_details adjusts the details of this airspace instance to 'detail'.
  void set_details(const Emergency &detail);

 private:
  struct Invalid {};

  union Details {
    Details();
    ~Details();

    Invalid invalid;
    Airport airport;
    ControlledAirspace controlled_airspace;
    Emergency emergency;
    Fire fire;
    Heliport heliport;
    Hospital hospital;
    Park park;
    PowerPlant power_plant;
    Prison prison;
    School school;
    SpecialUseAirspace special_use_airspace;
    TemporaryFlightRestriction tfr;
    Wildfire wildfire;
  };

  void reset();

  Id id_;
  std::string name_;
  Type type_;
  std::string country_;  // TODO(tvoss): Investigate constraints on country names.
  std::string state_;    // TODO(tvoss): Investigate constraints on state names.
  std::string city_;
  Timestamp last_updated_;
  Geometry geometry_;
  std::vector<Geometry> related_geometries_;
  std::vector<Rule> rules_;
  Details details_;
};

/// @cond
bool operator==(const Airport &lhs, const Airport &rhs);
bool operator==(const Airport::Runway &lhs, const Airport::Runway &rhs);
bool operator==(const ControlledAirspace &lhs, const ControlledAirspace &rhs);
bool operator==(const SpecialUseAirspace &lhs, const SpecialUseAirspace &rhs);
bool operator==(const TemporaryFlightRestriction &lhs, const TemporaryFlightRestriction &rhs);
bool operator==(const Wildfire &lhs, const Wildfire &rhs);
bool operator==(const Park &lhs, const Park &rhs);
bool operator==(const Prison &lhs, const Prison &rhs);
bool operator==(const School &lhs, const School &rhs);
bool operator==(const Hospital &lhs, const Hospital &rhs);
bool operator==(const Fire &lhs, const Fire &rhs);
bool operator==(const Emergency &lhs, const Emergency &rhs);
bool operator==(const Heliport &lhs, const Heliport &rhs);
bool operator==(const PowerPlant &lhs, const PowerPlant &rhs);

Airspace::Type operator~(Airspace::Type);
Airspace::Type operator|(Airspace::Type, Airspace::Type);
Airspace::Type operator&(Airspace::Type, Airspace::Type);

std::ostream &operator<<(std::ostream &, const Airspace &);
std::ostream &operator<<(std::ostream &, Airspace::Type);
/// @endcond

}  // namespace airmap

#endif  // AIRMAP_AIRSPACE_H_
