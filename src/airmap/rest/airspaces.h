#ifndef AIRMAP_REST_AIRSPACES_H_
#define AIRMAP_REST_AIRSPACES_H_

#include <airmap/airspaces.h>

#include <airmap/rest/communicator.h>

namespace airmap {
namespace rest {

class Airspaces : public airmap::Airspaces {
 public:
  explicit Airspaces(Communicator& communicator);
  Airspaces(const Airspaces&) = delete;
  Airspaces(Airspaces&&)      = delete;
  Airspaces& operator=(const Airspaces&) = delete;
  Airspaces& operator=(Airspaces&&) = delete;

  void search(const Search::Parameters& parameters, const Search::Callback& cb) override;
  void for_ids(const ForIds::Parameters& parameters, const ForIds::Callback& cb) override;

 private:
  Communicator& communicator_;
};

}  // namespace rest
}  // namespace airmap

#endif  // AIRMAP_REST_AIRSPACES_H_
