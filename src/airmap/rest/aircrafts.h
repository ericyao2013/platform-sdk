#ifndef AIRMAP_REST_AIRCRAFTS_H_
#define AIRMAP_REST_AIRCRAFTS_H_

#include <airmap/aircrafts.h>

#include <airmap/rest/communicator.h>

namespace airmap {
namespace rest {

class Aircrafts : public airmap::Aircrafts {
 public:
  explicit Aircrafts(Communicator& communicator);
  Aircrafts(const Aircrafts&) = delete;
  Aircrafts(Aircrafts&&)      = delete;
  Aircrafts& operator=(const Aircrafts&) = delete;
  Aircrafts& operator=(Aircrafts&&) = delete;

  void manufacturers(const Manufacturers::Parameters& parameters, const Manufacturers::Callback& cb) override;
  void models(const Models::Parameters& parameters, const Models::Callback& cb) override;
  void model_for_id(const ModelForId::Parameters& parameters, const ModelForId::Callback& cb) override;

 private:
  Communicator& communicator_;
};

}  // namespace rest
}  // namespace airmap

#endif  // AIRMAP_REST_AIRCRAFTS_H_
