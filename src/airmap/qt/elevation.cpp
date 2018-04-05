#include <airmap/qt/elevation.h>

std::shared_ptr<airmap::qt::Elevation> airmap::qt::Elevation::create(const std::shared_ptr<Dispatcher>& dispatcher,
                                                                     const std::shared_ptr<airmap::Client>& client) {
  return std::shared_ptr<Elevation>{new Elevation{dispatcher, client}};
}

airmap::qt::Elevation::Elevation(const std::shared_ptr<Dispatcher>& dispatcher,
                                 const std::shared_ptr<airmap::Client>& client)
    : dispatcher_{dispatcher}, client_{client} {
}

void airmap::qt::Elevation::get_elevation_points(const GetElevation::Parameters& parameters,
                                                 const GetElevation::Callback& cb) {
  dispatcher_->dispatch_to_native([ this, sp = shared_from_this(), parameters, cb ]() {
    sp->client_->elevation().get_elevation_points(parameters, [this, sp, cb](const auto& result) {
      sp->dispatcher_->dispatch_to_qt([sp, result, cb]() { cb(result); });
    });
  });
}
