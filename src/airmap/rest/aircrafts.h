#ifndef AIRMAP_REST_AIRCRAFTS_H_
#define AIRMAP_REST_AIRCRAFTS_H_

#include <airmap/aircrafts.h>

#include <airmap/rest/communicator.h>

namespace airmap {
namespace rest {

class Aircrafts : public airmap::Aircrafts {
 public:
  explicit Aircrafts(const std::string& host, Client::Version version, Communicator& communicator);

  void manufacturers(const Manufacturers::Parameters& parameters, const Manufacturers::Callback& cb) override;
  void models(const Models::Parameters& parameters, const Models::Callback& cb) override;
  void model_for_id(const ModelForId::Parameters& parameters, const ModelForId::Callback& cb) override;

 private:
  std::string host_;
  Client::Version version_;
  Communicator& communicator_;
};

}  // namespace rest
}  // namespace airmap

#endif  // AIRMAP_REST_AIRCRAFTS_H_
