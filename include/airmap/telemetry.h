#ifndef AIRMAP_TELEMETRY_H_
#define AIRMAP_TELEMETRY_H_

#include <cstdint>
#include <string>
#include <type_traits>

namespace airmap {

struct Flight;

class Telemetry {
 public:
  struct Position {
    std::uint64_t timestamp;
    double latitude;
    double longitude;
    float altitude_msl;
    float altitude_gl;
    float horizontal_accuracy;
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

  Telemetry(const Telemetry&) = delete;
  Telemetry(Telemetry&&) = delete;
  virtual ~Telemetry() = default;
  Telemetry& operator=(const Telemetry&) = delete;
  Telemetry& operator=(Telemetry&&) = delete;

  /// submit_updates sends the telemetry data in 'updates' associated to 'flight' to the AirMap
  /// services.
  virtual void submit_updates(const Flight& flight, const std::string& key,
                              const std::initializer_list<Update>& updates) = 0;

 protected:
  Telemetry() = default;
};

}  // namespace airmap

#endif  // AIRMAP_TELEMETRY_H_
