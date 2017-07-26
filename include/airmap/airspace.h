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

struct Airport {
  struct Runway {
    std::string name;
    float length;
    float bearing;
  };

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

struct ControlledAirspace {};

struct SpecialUseAirspace {
  enum class Type {};
  Type type;
};

struct TemporaryFlightRestriction {
  enum class Type {};
  std::string url;
  Type type;
  std::string reason;
};

struct Wildfire {
  std::string effective_date;
};

struct Park {};

struct Prison {};

struct School {};

struct Hospital {};

struct Fire {};

struct Emergency {};

struct Heliport {
  enum class Usage {};
  std::string faa_id;
  std::string phone;
  Usage usage;
};

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

  Airspace();
  Airspace(const Airspace &rhs);
  ~Airspace();
  Airspace &operator=(const Airspace &rhs);
  bool operator==(const Airspace &rhs) const;
  bool operator!=(const Airspace &rhs) const;

  const Id &id() const;
  void set_id(const Id &id);

  const std::string &name() const;
  void set_name(const std::string &name);

  Type type() const;

  const std::string &country() const;
  void set_country(const std::string &country);

  const std::string &state() const;
  void set_state(const std::string &state);

  const std::string &city() const;
  void set_city(const std::string &city);

  const Timestamp &last_updated() const;
  void set_last_updated(const Timestamp &timestamp);

  const Geometry &geometry() const;
  void set_geometry(const Geometry &geometry);

  const std::vector<Geometry> &related_geometries() const;
  void set_related_geometries(const std::vector<Geometry> &geometries);

  const std::vector<Rule> &rules() const;
  void set_rules(const std::vector<Rule> &rules);

  const Airport &details_for_airport() const;
  Airport &details_for_airport();

  const ControlledAirspace &details_for_controlled_airspace() const;
  ControlledAirspace &details_for_controlled_airspace();

  const Emergency &details_for_emergency() const;
  Emergency &details_for_emergency();

  const Fire &details_for_fire() const;
  Fire &details_for_fire();

  const Heliport &details_for_heliport() const;
  Heliport &details_for_heliport();

  const Hospital &details_for_hospital() const;
  Hospital &details_for_hospital();

  const Park &details_for_park() const;
  Park &details_for_park();

  const PowerPlant &details_for_power_plant() const;
  PowerPlant &details_for_power_plant();

  const Prison &details_for_prison() const;
  Prison &details_for_prison();

  const School &details_for_school() const;
  School &details_for_school();

  const SpecialUseAirspace &details_for_special_use_airspace() const;
  SpecialUseAirspace &details_for_special_use_airspace();

  const TemporaryFlightRestriction &details_for_temporary_flight_restriction() const;
  TemporaryFlightRestriction &details_for_temporary_flight_restriction();

  const Wildfire &details_for_wildfire() const;
  Wildfire &details_for_wildfire();

  void set_details(const Airspace &detail);
  void set_details(const Airport &detail);
  void set_details(const ControlledAirspace &detail);
  void set_details(const SpecialUseAirspace &detail);
  void set_details(const TemporaryFlightRestriction &detail);
  void set_details(const Wildfire &detail);
  void set_details(const Park &detail);
  void set_details(const PowerPlant &detail);
  void set_details(const Heliport &detail);
  void set_details(const Prison &detail);
  void set_details(const School &detail);
  void set_details(const Hospital &detail);
  void set_details(const Fire &detail);
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

}  // namespace airmap

#endif  // AIRMAP_AIRSPACE_H_
