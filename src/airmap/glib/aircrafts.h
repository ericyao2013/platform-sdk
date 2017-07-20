#ifndef AIRMAP_GLIB_AIRCRAFTS_H_
#define AIRMAP_GLIB_AIRCRAFTS_H_

#include <airmap/aircrafts.h>

#include <airmap/glib/api.h>

namespace airmap {
namespace glib {

class Aircrafts : public airmap::Aircrafts {
 public:
  explicit Aircrafts(Api& api);
  Aircrafts(const Aircrafts&) = delete;
  Aircrafts(Aircrafts&&)      = delete;
  Aircrafts& operator=(const Aircrafts&) = delete;
  Aircrafts& operator=(Aircrafts&&) = delete;

  void manufacturers(const Manufacturers::Parameters& parameters,
                     const Manufacturers::Callback& cb) override;
  void models(const Models::Parameters& parameters, const Models::Callback& cb) override;
  void model_for_id(const ModelForId::Parameters& parameters,
                    const ModelForId::Callback& cb) override;

 private:
  Api& api_;
};

}  // namespace glib
}  // namespace airmap

#endif  // AIRMAP_GLIB_AIRCRAFTS_H_
