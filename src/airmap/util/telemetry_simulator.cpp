#include <airmap/util/telemetry_simulator.h>

#include <iostream>

airmap::util::TelemetrySimulator::TelemetrySimulator(const airmap::Geometry::Polygon& polygon)
    : polygon_{polygon},
      segment_begin_{polygon_.at(0).coordinates.begin()},
      segment_end_{polygon_.at(0).coordinates.begin() + 1},
      segment_ruler_{segment_begin_->latitude},
      segment_start_time_{Clock::universal_time()},
      segment_bearing_{segment_ruler_.bearing(*segment_begin_, *segment_end_)},
      segment_length_{segment_ruler_.distance(*segment_begin_, *segment_end_)},
      current_data_{*segment_begin_},
      current_time_{Clock::universal_time()} {
}

airmap::Geometry::Coordinate airmap::util::TelemetrySimulator::update(const DateTime& now) {
  if (segment_ruler_.distance(*segment_begin_, current_data_) > segment_length_) {
    segment_begin_ = segment_end_;
    segment_end_   = segment_end_ + 1;

    if (segment_end_ == polygon_.at(0).coordinates.end()) {
      segment_begin_ = polygon_.at(0).coordinates.begin();
      segment_end_   = segment_begin_ + 1;
    }

    segment_ruler_      = CheapRuler{segment_begin_->latitude};
    segment_length_     = segment_ruler_.distance(*segment_begin_, *segment_end_);
    segment_bearing_    = segment_ruler_.bearing(*segment_begin_, *segment_end_);
    segment_start_time_ = Clock::universal_time();
    current_data_       = *segment_begin_;
  }

  auto dt  = now - current_time_;
  current_data_ = segment_ruler_.destination(current_data_, dt.total_milliseconds() / 1E3 * 2., segment_bearing_);
  current_time_ = now;

  return current_data_;
}
