#include <airmap/airspace.h>

#include <iostream>

namespace {

struct Comma {
  bool first = true;
};

std::ostream &operator<<(std::ostream &out, Comma &comma) {
  if (not comma.first)
    out << ",";
  comma.first = false;
  return out;
}

}  // namespace

airmap::Airspace::Airspace() : type_{Type::invalid} {
}

airmap::Airspace::Airspace(const Airspace &rhs)
    : id_{rhs.id_},
      name_{rhs.name_},
      type_{Type::invalid},
      country_{rhs.country_},
      state_{rhs.state_},
      city_{rhs.city_},
      last_updated_{rhs.last_updated_},
      geometry_{rhs.geometry_},
      related_geometries_{rhs.related_geometries_},
      rules_{rhs.rules_} {
  set_details(rhs);
}

airmap::Airspace::~Airspace() {
  reset();
}

airmap::Airspace &airmap::Airspace::operator=(const Airspace &rhs) {
  reset();

  id_                 = rhs.id_;
  name_               = rhs.name_;
  type_               = rhs.type_;
  country_            = rhs.country_;
  state_              = rhs.state_;
  city_               = rhs.city_;
  last_updated_       = rhs.last_updated_;
  geometry_           = rhs.geometry_;
  related_geometries_ = rhs.related_geometries_;
  rules_              = rhs.rules_;

  set_details(rhs);

  return *this;
}

const airmap::Airspace::Id &airmap::Airspace::id() const {
  return id_;
}

void airmap::Airspace::set_id(const Id &id) {
  id_ = id;
}

const std::string &airmap::Airspace::name() const {
  return name_;
}

void airmap::Airspace::set_name(const std::string &name) {
  name_ = name;
}

airmap::Airspace::Type airmap::Airspace::type() const {
  return type_;
}

const std::string &airmap::Airspace::country() const {
  return country_;
}

void airmap::Airspace::set_country(const std::string &country) {
  country_ = country;
}

const std::string &airmap::Airspace::state() const {
  return state_;
}

void airmap::Airspace::set_state(const std::string &state) {
  state_ = state;
}

const std::string &airmap::Airspace::city() const {
  return city_;
}

void airmap::Airspace::set_city(const std::string &city) {
  city_ = city;
}

const airmap::Timestamp &airmap::Airspace::last_updated() const {
  return last_updated_;
}

void airmap::Airspace::set_last_updated(const Timestamp &timestamp) {
  last_updated_ = timestamp;
}

const airmap::Geometry &airmap::Airspace::geometry() const {
  return geometry_;
}

void airmap::Airspace::set_geometry(const Geometry &geometry) {
  geometry_ = geometry;
}

const std::vector<airmap::Geometry> &airmap::Airspace::related_geometries() const {
  return related_geometries_;
}

void airmap::Airspace::set_related_geometries(const std::vector<Geometry> &geometries) {
  related_geometries_ = geometries;
}

const std::vector<airmap::Rule> &airmap::Airspace::rules() const {
  return rules_;
}

void airmap::Airspace::set_rules(const std::vector<Rule> &rules) {
  rules_ = rules;
}

const airmap::Airport &airmap::Airspace::details_for_airport() const {
  return details_.airport;
}

airmap::Airport &airmap::Airspace::details_for_airport() {
  return details_.airport;
}

const airmap::ControlledAirspace &airmap::Airspace::details_for_controlled_airspace() const {
  return details_.controlled_airspace;
}

airmap::ControlledAirspace &airmap::Airspace::details_for_controlled_airspace() {
  return details_.controlled_airspace;
}

const airmap::Emergency &airmap::Airspace::details_for_emergency() const {
  return details_.emergency;
}

airmap::Emergency &airmap::Airspace::details_for_emergency() {
  return details_.emergency;
}

const airmap::Fire &airmap::Airspace::details_for_fire() const {
  return details_.fire;
}

airmap::Fire &airmap::Airspace::details_for_fire() {
  return details_.fire;
}

const airmap::Heliport &airmap::Airspace::details_for_heliport() const {
  return details_.heliport;
}

airmap::Heliport &airmap::Airspace::details_for_heliport() {
  return details_.heliport;
}

const airmap::Hospital &airmap::Airspace::details_for_hospital() const {
  return details_.hospital;
}

airmap::Hospital &airmap::Airspace::details_for_hospital() {
  return details_.hospital;
}

const airmap::Park &airmap::Airspace::details_for_park() const {
  return details_.park;
}

airmap::Park &airmap::Airspace::details_for_park() {
  return details_.park;
}

const airmap::PowerPlant &airmap::Airspace::details_for_power_plant() const {
  return details_.power_plant;
}

airmap::PowerPlant &airmap::Airspace::details_for_power_plant() {
  return details_.power_plant;
}

const airmap::Prison &airmap::Airspace::details_for_prison() const {
  return details_.prison;
}

airmap::Prison &airmap::Airspace::details_for_prison() {
  return details_.prison;
}

const airmap::School &airmap::Airspace::details_for_school() const {
  return details_.school;
}

airmap::School &airmap::Airspace::details_for_school() {
  return details_.school;
}

const airmap::SpecialUseAirspace &airmap::Airspace::details_for_special_use_airspace() const {
  return details_.special_use_airspace;
}

airmap::SpecialUseAirspace &airmap::Airspace::details_for_special_use_airspace() {
  return details_.special_use_airspace;
}

const airmap::TemporaryFlightRestriction &
airmap::Airspace::details_for_temporary_flight_restriction() const {
  return details_.tfr;
}

airmap::TemporaryFlightRestriction &airmap::Airspace::details_for_temporary_flight_restriction() {
  return details_.tfr;
}

const airmap::Wildfire &airmap::Airspace::details_for_wildfire() const {
  return details_.wildfire;
}

airmap::Wildfire &airmap::Airspace::details_for_wildfire() {
  return details_.wildfire;
}

void airmap::Airspace::set_details(const Airspace &detail) {
  switch (detail.type_) {
    case Type::airport:
      set_details(detail.details_for_airport());
      break;
    case Type::controlled_airspace:
      set_details(detail.details_for_controlled_airspace());
      break;
    case Type::special_use_airspace:
      set_details(detail.details_for_special_use_airspace());
      break;
    case Type::tfr:
      set_details(detail.details_for_temporary_flight_restriction());
      break;
    case Type::wildfire:
      set_details(detail.details_for_wildfire());
      break;
    case Type::park:
      set_details(detail.details_for_park());
      break;
    case Type::power_plant:
      set_details(detail.details_for_power_plant());
      break;
    case Type::heliport:
      set_details(detail.details_for_heliport());
      break;
    case Type::prison:
      set_details(detail.details_for_prison());
      break;
    case Type::school:
      set_details(detail.details_for_school());
      break;
    case Type::hospital:
      set_details(detail.details_for_hospital());
      break;
    case Type::fire:
      set_details(detail.details_for_fire());
      break;
    case Type::emergency:
      set_details(detail.details_for_emergency());
      break;
    default:
      break;
  }

  type_ = detail.type_;
}

void airmap::Airspace::set_details(const Airport &detail) {
  type_ = Type::airport;
  new (&details_.airport) Airport(detail);
}

void airmap::Airspace::set_details(const ControlledAirspace &detail) {
  type_ = Type::controlled_airspace;
  new (&details_.controlled_airspace) ControlledAirspace{detail};
}

void airmap::Airspace::set_details(const SpecialUseAirspace &detail) {
  type_ = Type::special_use_airspace;
  new (&details_.special_use_airspace) SpecialUseAirspace{detail};
}

void airmap::Airspace::set_details(const TemporaryFlightRestriction &detail) {
  type_ = Type::tfr;
  new (&details_.tfr) TemporaryFlightRestriction(detail);
}

void airmap::Airspace::set_details(const Wildfire &detail) {
  type_ = Type::wildfire;
  new (&details_.wildfire) Wildfire{detail};
}

void airmap::Airspace::set_details(const Park &detail) {
  type_ = Type::park;
  new (&details_.park) Park{detail};
}

void airmap::Airspace::set_details(const PowerPlant &detail) {
  type_ = Type::power_plant;
  new (&details_.power_plant) PowerPlant{detail};
}

void airmap::Airspace::set_details(const Heliport &detail) {
  type_ = Type::heliport;
  new (&details_.heliport) Heliport{detail};
}

void airmap::Airspace::set_details(const Prison &detail) {
  type_ = Type::prison;
  new (&details_.prison) Prison{detail};
}

void airmap::Airspace::set_details(const School &detail) {
  type_ = Type::school;
  new (&details_.school) School{detail};
}

void airmap::Airspace::set_details(const Hospital &detail) {
  type_ = Type::hospital;
  new (&details_.hospital) Hospital{detail};
}

void airmap::Airspace::set_details(const Fire &detail) {
  type_ = Type::fire;
  new (&details_.fire) Fire{detail};
}

void airmap::Airspace::set_details(const Emergency &detail) {
  type_ = Type::emergency;
  new (&details_.emergency) Emergency{detail};
}

void airmap::Airspace::reset() {
  switch (type_) {
    case Type::airport:
      details_.airport.~Airport();
      break;
    case Type::controlled_airspace:
      details_.controlled_airspace.~ControlledAirspace();
      break;
    case Type::special_use_airspace:
      details_.special_use_airspace.~SpecialUseAirspace();
      break;
    case Type::tfr:
      details_.tfr.~TemporaryFlightRestriction();
      break;
    case Type::wildfire:
      details_.wildfire.~Wildfire();
      break;
    case Type::park:
      details_.park.~Park();
      break;
    case Type::power_plant:
      details_.power_plant.~PowerPlant();
      break;
    case Type::heliport:
      details_.heliport.~Heliport();
      break;
    case Type::prison:
      details_.prison.~Prison();
      break;
    case Type::school:
      details_.school.~School();
      break;
    case Type::hospital:
      details_.hospital.~Hospital();
      break;
    case Type::fire:
      details_.fire.~Fire();
      break;
    case Type::emergency:
      details_.emergency.~Emergency();
      break;
    default:
      break;
  }
}

airmap::Airspace::Details::Details() : invalid{} {
}
airmap::Airspace::Details::~Details() {
}

airmap::Airspace::Type airmap::operator~(airmap::Airspace::Type type) {
  using UT = typename std::underlying_type<Airspace::Type>::type;
  return static_cast<Airspace::Type>(~static_cast<UT>(type));
}

airmap::Airspace::Type airmap::operator|(airmap::Airspace::Type lhs, airmap::Airspace::Type rhs) {
  using UT = typename std::underlying_type<Airspace::Type>::type;
  return static_cast<Airspace::Type>(static_cast<UT>(lhs) | static_cast<UT>(rhs));
}

airmap::Airspace::Type airmap::operator&(airmap::Airspace::Type lhs, airmap::Airspace::Type rhs) {
  using UT = typename std::underlying_type<Airspace::Type>::type;
  return static_cast<Airspace::Type>(static_cast<UT>(lhs) & static_cast<UT>(rhs));
}

std::ostream &airmap::operator<<(std::ostream &out, const Airspace &) {
  return out;
}

std::ostream &airmap::operator<<(std::ostream &out, Airspace::Type types) {
  if (types == airmap::Airspace::Type::invalid)
    return out;

  Comma comma;

  if ((types & airmap::Airspace::Type::airport) == airmap::Airspace::Type::airport)
    out << comma << "airport";
  if ((types & airmap::Airspace::Type::controlled_airspace) ==
      airmap::Airspace::Type::controlled_airspace)
    out << comma << "controlled_airspace";
  if ((types & airmap::Airspace::Type::special_use_airspace) ==
      airmap::Airspace::Type::special_use_airspace)
    out << comma << "special_use_airspace";
  if ((types & airmap::Airspace::Type::tfr) == airmap::Airspace::Type::tfr)
    out << comma << "tfr";
  if ((types & airmap::Airspace::Type::wildfire) == airmap::Airspace::Type::wildfire)
    out << comma << "wildfire";
  if ((types & airmap::Airspace::Type::park) == airmap::Airspace::Type::park)
    out << comma << "park";
  if ((types & airmap::Airspace::Type::power_plant) == airmap::Airspace::Type::power_plant)
    out << comma << "power_plant";
  if ((types & airmap::Airspace::Type::heliport) == airmap::Airspace::Type::heliport)
    out << comma << "heliport";
  if ((types & airmap::Airspace::Type::prison) == airmap::Airspace::Type::prison)
    out << comma << "prison";
  if ((types & airmap::Airspace::Type::school) == airmap::Airspace::Type::school)
    out << comma << "school";
  if ((types & airmap::Airspace::Type::hospital) == airmap::Airspace::Type::hospital)
    out << comma << "hospital";
  if ((types & airmap::Airspace::Type::fire) == airmap::Airspace::Type::fire)
    out << comma << "fire";
  if ((types & airmap::Airspace::Type::emergency) == airmap::Airspace::Type::emergency)
    out << comma << "emergency";

  return out;
}