#ifndef AIRMAP_GLIB_AIRSPACES_H_
#define AIRMAP_GLIB_AIRSPACES_H_

#include <airmap/airspaces.h>

#include <airmap/glib/api.h>

namespace airmap {
namespace glib {

class Airspaces : public airmap::Airspaces {
 public:
  explicit Airspaces(Api& api);
  Airspaces(const Airspaces&) = delete;
  Airspaces(Airspaces&&)      = delete;
  Airspaces& operator=(const Airspaces&) = delete;
  Airspaces& operator=(Airspaces&&) = delete;

  void search(const Search::Parameters& parameters, const Search::Callback& cb) override;
  void for_ids(const ForIds::Parameters& parameters, const ForIds::Callback& cb) override;

 private:
  Api& api_;
};

}  // namespace glib
}  // namespace airmap

#endif  // AIRMAP_GLIB_AIRSPACES_H_
