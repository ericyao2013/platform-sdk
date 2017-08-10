#ifndef AIRMAP_TELEMETRY_H_
#define AIRMAP_TELEMETRY_H_

#include <airmap/do_not_copy_or_move.h>

#include <cstdint>
#include <string>
#include <type_traits>

namespace airmap {

struct Flight;

class Telemetry : DoNotCopyOrMove {
 public:
  struct Position {
    std::uint64_t timestamp;
    double latitude;
    double longitude;
    double altitude_msl;
    double altitude_gl;
    double horizontal_accuracy;
  };

  struct Speed {
    std::uint64_t timestamp;
    float velocity_x;
    float velocity_y;
    float velocity_z;
  };

  struct Attitude {
    std::uint64_t timestamp;
    float yaw;
    float pitch;
    float roll;
  };

  struct Barometer {
    std::uint64_t timestamp;
    float pressure;
  };

  static_assert(std::is_pod<Position>::value, "Position must be a POD");
  static_assert(std::is_pod<Speed>::value, "Speed must be a POD");
  static_assert(std::is_pod<Attitude>::value, "Attitude must be a POD");
  static_assert(std::is_pod<Barometer>::value, "Barometer must be a POD");

  class Update {
   public:
    enum class Type : std::uint8_t { position = 1, speed = 2, attitude = 3, barometer = 4 };

    explicit Update(const Position& position);
    explicit Update(const Speed& speed);
    explicit Update(const Attitude& attitude);
    explicit Update(const Barometer& barometer);

    Type type() const;
    const Position& position() const;
    const Speed& speed() const;
    const Attitude& attitude() const;
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
  Telemetry() = default;
};

}  // namespace airmap

#endif  // AIRMAP_TELEMETRY_H_
