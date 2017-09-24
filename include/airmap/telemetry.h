#ifndef AIRMAP_TELEMETRY_H_
#define AIRMAP_TELEMETRY_H_

#include <airmap/do_not_copy_or_move.h>

#include <cstdint>
#include <string>
#include <type_traits>

namespace airmap {

struct Flight;

/// Telemetry provides functionality to submit telemetry updates to the
/// AirMap services during flight.
class Telemetry : DoNotCopyOrMove {
 public:
  /// Position describes a timestamped geographical position.
  struct Position {
    std::uint64_t timestamp;
    double latitude;
    double longitude;
    double altitude_msl;
    double altitude_gl;
    double horizontal_accuracy;
  };

  /// Speed describes the timestamped 3-dim velocity of a vehicle.
  struct Speed {
    std::uint64_t timestamp;
    float velocity_x;
    float velocity_y;
    float velocity_z;
  };

  /// Attitude describes the timestamped 3-dim orientation of a vehicle.
  struct Attitude {
    std::uint64_t timestamp;
    float yaw;
    float pitch;
    float roll;
  };

  /// Barometer describes the timestamped atmospheric pressurce conditions.
  struct Barometer {
    std::uint64_t timestamp;
    float pressure;
  };

  /// @cond
  static_assert(std::is_pod<Position>::value, "Position must be a POD");
  static_assert(std::is_pod<Speed>::value, "Speed must be a POD");
  static_assert(std::is_pod<Attitude>::value, "Attitude must be a POD");
  static_assert(std::is_pod<Barometer>::value, "Barometer must be a POD");
  /// @endcond

  /// Update models an update of the current position, speed, attitude or atmospheric pressure
  /// measurement/estimate.
  class Update {
   public:
    /// Type enumerates all known update types.
    enum class Type : std::uint8_t { position = 1, speed = 2, attitude = 3, barometer = 4 };

    /// Update initializes a new instance with 'position'.
    explicit Update(const Position& position);
    /// Update initializes a new instance with 'speed'.
    explicit Update(const Speed& speed);
    /// Update initializes a new instance with 'attitude'.
    explicit Update(const Attitude& attitude);
    /// Update initializes a new instance with 'barometer'.
    explicit Update(const Barometer& barometer);

    /// type returns the Type of the update contained within this instance.
    Type type() const;
    /// position returns the position update contained within this instance.
    /// The behavior is undefined if type() != Type::position.
    const Position& position() const;
    /// speed returns the speed update contained within this instance.
    /// The behavior is undefined if type() != Type::speed.
    const Speed& speed() const;
    /// attitude returns the attitde update contained within this instance.
    /// The behavior is undefined if type() != Type::attitude.
    const Attitude& attitude() const;
    /// barometer returns the barometer update contained within this instance.
    /// The behavior is undefined if type() != Type::barometer.
    const Barometer& barometer() const;

   private:
    Type type_;
    union {
      Position position;
      Speed speed;
      Attitude attitude;
      Barometer barometer;
    } data_;
  };

  /// submit_updates sends the telemetry data in 'updates' associated to 'flight' to the AirMap
  /// services.
  virtual void submit_updates(const Flight& flight, const std::string& key,
                              const std::initializer_list<Update>& updates) = 0;

 protected:
  /// @cond
  Telemetry() = default;
  /// @endcond
};

}  // namespace airmap

#endif  // AIRMAP_TELEMETRY_H_
