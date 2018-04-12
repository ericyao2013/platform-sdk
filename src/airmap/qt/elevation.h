#ifndef AIRMAP_QT_ELEVATION_H_
#define AIRMAP_QT_ELEVATION_H_

#include <airmap/client.h>
#include <airmap/elevation.h>
#include <airmap/qt/dispatcher.h>

#include <memory>

namespace airmap {
namespace qt {

class Elevation : public airmap::Elevation, public std::enable_shared_from_this<Elevation> {
 public:
  static std::shared_ptr<Elevation> create(const std::shared_ptr<Dispatcher>& dispatcher,
                                           const std::shared_ptr<airmap::Client>& client);

  void get_elevation_points(const GetElevationPoints::Parameters& parameters, const GetElevationPoints::Callback& cb) override;

 private:
  explicit Elevation(const std::shared_ptr<Dispatcher>& dispatcher, const std::shared_ptr<airmap::Client>& client);

  std::shared_ptr<Dispatcher> dispatcher_;
  std::shared_ptr<airmap::Client> client_;
};

}  // namespace qt
}  // namespace airmap

#endif  // AIRMAP_QT_ELEVATION_H_
