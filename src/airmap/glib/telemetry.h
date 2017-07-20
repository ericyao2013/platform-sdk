#ifndef AIRMAP_GLIB_TELEMETRY_H_
#define AIRMAP_GLIB_TELEMETRY_H_

#include <airmap/telemetry.h>

#include <airmap/glib/api.h>
#include <botan/auto_rng.h>

#include <memory>

namespace airmap {
namespace glib {

class Telemetry : public airmap::Telemetry {
 public:
  explicit Telemetry(Api& api);

  void submit_updates(const Flight& flight, const std::string& key,
                      const std::initializer_list<Update>& updates) override;

 private:
  Api& api_;
  Botan::AutoSeeded_RNG rng_;
};

}  // namespace glib
}  // namespace airmap

#endif  // AIRMAP_GLIB_TELEMETRY_H_
