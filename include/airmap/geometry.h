#ifndef AIRMAP_GEOMETRY_H_
#define AIRMAP_GEOMETRY_H_

#include <airmap/optional.h>

#include <vector>

namespace airmap {

class Geometry {
 public:
  enum class Type {
    invalid,
    point,
    multi_point,
    line_string,
    multi_line_string,
    polygon,
    multi_polygon,
    geometry_collection
  };

  struct Coordinate {
    double latitude;
    double longitude;
    Optional<double> altitude;
    Optional<double> elevation;
  };

  template <Type tag>
  struct CoordinateVector {
    std::vector<Coordinate> coordinates;
  };

  using Point              = Coordinate;
  using MultiPoint         = CoordinateVector<Type::multi_point>;
  using LineString         = CoordinateVector<Type::line_string>;
  using MultiLineString    = std::vector<LineString>;
  using Polygon            = std::vector<CoordinateVector<Type::polygon>>;
  using MultiPolygon       = std::vector<Polygon>;
  using GeometryCollection = std::vector<Geometry>;

  static Geometry point(double lat, double lon);
  static Geometry polygon(const std::vector<Coordinate>& coordinates);

  Geometry();
  explicit Geometry(const Point& other);
  explicit Geometry(const MultiPoint& other);
  explicit Geometry(const LineString& other);
  explicit Geometry(const MultiLineString& other);
  explicit Geometry(const Polygon& other);
  explicit Geometry(const MultiPolygon& other);
  explicit Geometry(const GeometryCollection& other);
  Geometry(const Geometry& other);
  ~Geometry();

  Geometry& operator=(const Geometry& rhs);
  bool operator==(const Geometry& rhs) const;

  Type type() const;
  const Point& details_for_point() const;
  const MultiPoint& details_for_multi_point() const;
  const LineString& details_for_line_string() const;
  const MultiLineString& details_for_multi_line_string() const;
  const Polygon& details_for_polygon() const;
  const MultiPolygon& details_for_multi_polygon() const;
  const GeometryCollection details_for_geometry_collection() const;

 private:
  struct Invalid {};

  union Data {
    Data();
    ~Data();

    Invalid invalid;
    Point point;
    MultiPoint multi_point;
    LineString line_string;
    MultiLineString multi_line_string;
    Polygon polygon;
    MultiPolygon multi_polygon;
    GeometryCollection geometry_collection;
  };

  Geometry& reset();
  void set_point(const Point& point);
  void set_multi_point(const MultiPoint& multi_point);
  void set_line_string(const LineString& line_string);
  void set_multi_line_string(const MultiLineString& multi_line_string);
  void set_polygon(const Polygon& polygon);
  void set_multi_polygon(const MultiPolygon& multi_polygon);
  void set_geometry_collection(const GeometryCollection& geometry_collection);
  Geometry& set_geometry(const Geometry& other);

  Type type_;
  Data data_;
};

}  // namespace airmap

#endif  // AIRMAP_GEOMETRY_H_
