#include <airmap/glib/telemetry.h>

#include <airmap/flight.h>

#include "telemetry.pb.h"

#include <botan/base64.h>
#include <botan/key_filt.h>
#include <botan/pipe.h>

#include <arpa/inet.h>

#include <cstdint>
#include <sstream>

namespace {

class Buffer {
 public:
  template <typename T>
  Buffer& add(const T& value) {
    return add(reinterpret_cast<const std::uint8_t*>(&value), sizeof(value));
  }

  Buffer& add(const std::string& s) {
    return add(reinterpret_cast<const std::uint8_t*>(s.c_str()), s.size());
  }

  Buffer& add(const std::vector<std::uint8_t>& v) {
    return add(v.data(), v.size());
  }

  Buffer& add(const Botan::secure_vector<std::uint8_t>& v) {
    return add(v.data(), v.size());
  }

  Buffer& add(const std::uint8_t* value, std::size_t size) {
    buffer_.insert(buffer_.end(), value, value + size);
    return *this;
  }

  Botan::secure_vector<std::uint8_t> finalize_as_secure_vector() {
    return Botan::secure_vector<std::uint8_t>{buffer_.begin(), buffer_.end()};
  }

  std::vector<std::uint8_t> finalize_as_vector() {
    return buffer_;
  }

 private:
  std::vector<std::uint8_t> buffer_;
};

namespace telemetry {

constexpr const char* host{"192.168.178.24"};
constexpr std::uint8_t encryption_type{1};
constexpr const char* encryption_spec{"AES-256/CBC/PKCS7"};
constexpr std::uint16_t port{16060};

}  // namespace telemetry
}  // namespace

airmap::glib::Telemetry::Telemetry(Api& api) : api_{api} {
}

void airmap::glib::Telemetry::submit_updates(const Flight& flight, const std::string& key,
                                             const std::initializer_list<Update>& updates) {
  static std::uint32_t counter{0};

  Buffer payload;

  for (const auto& update : updates) {
    switch (update.type()) {
      case Update::Type::position: {
        airmap::telemetry::Position position;
        position.set_timestamp(update.position().timestamp);
        position.set_latitude(update.position().latitude);
        position.set_longitude(update.position().longitude);
        position.set_altitude_agl(update.position().altitude_gl);
        position.set_altitude_msl(update.position().altitude_msl);
        position.set_horizontal_accuracy(update.position().horizontal_accuracy);
        auto message = position.SerializeAsString();
        payload.add<std::uint16_t>(htons(static_cast<std::uint16_t>(Telemetry::Update::Type::position)))
            .add<std::uint16_t>(htons(message.size()))
            .add(message);
        break;
      }
      case Update::Type::speed: {
        airmap::telemetry::Speed speed;
        speed.set_timestamp(update.speed().timestamp);
        speed.set_velocity_x(update.speed().velocity_x);
        speed.set_velocity_y(update.speed().velocity_y);
        speed.set_velocity_z(update.speed().velocity_z);
        auto message = speed.SerializeAsString();
        payload.add<std::uint16_t>(htons(static_cast<std::uint16_t>(Telemetry::Update::Type::speed)))
            .add<std::uint16_t>(htons(message.size()))
            .add(message);
        break;
      }
      case Update::Type::attitude: {
        airmap::telemetry::Attitude attitude;
        attitude.set_timestamp(update.attitude().timestamp);
        attitude.set_yaw(update.attitude().yaw);
        attitude.set_pitch(update.attitude().pitch);
        attitude.set_roll(update.attitude().roll);
        auto message = attitude.SerializeAsString();
        payload.add<std::uint16_t>(htons(static_cast<std::uint16_t>(Telemetry::Update::Type::attitude)))
            .add<std::uint16_t>(htons(message.size()))
            .add(message);
        break;
      }
      case Update::Type::barometer: {
        airmap::telemetry::Barometer barometer;
        barometer.set_timestamp(update.barometer().timestamp);
        barometer.set_pressure(update.barometer().pressure);
        auto message = barometer.SerializeAsString();
        payload.add<std::uint16_t>(htons(static_cast<std::uint16_t>(Telemetry::Update::Type::barometer)))
            .add<std::uint16_t>(htons(message.size()))
            .add(message);
        break;
      }
    }
  }

  auto pl = payload.finalize_as_secure_vector();

  std::vector<std::uint8_t> iv(16, 0);
  rng_.randomize(iv.data(), iv.size());

  Botan::Pipe pipe(Botan::get_cipher(::telemetry::encryption_spec, Botan::base64_decode(key), iv, Botan::ENCRYPTION));
  pipe.process_msg(payload.finalize_as_secure_vector());

  Buffer packet;
  api_.send_udp(::telemetry::host, ::telemetry::port,
                packet.add<std::uint32_t>(htonl(counter++))
                    .add<std::uint8_t>(flight.id.size())
                    .add(flight.id)
                    .add<std::uint8_t>(::telemetry::encryption_type)
                    .add(iv)
                    .add(pipe.read_all())
                    .finalize_as_vector());
}