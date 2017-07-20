cmake_minimum_required(VERSION 3.5)
project(airmap)

find_package(Boost REQUIRED date_time)
find_package(PkgConfig)
find_package(Protobuf)

pkg_check_modules(BOTAN REQUIRED botan-2)
pkg_check_modules(LIBSOUP REQUIRED libsoup-2.4)
pkg_check_modules(GIO gio-2.0)
pkg_check_modules(GIO_UNIX gio-unix-2.0)

include_directories(
    include
    src

    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/src

    ${AWS_CPP_SDK_CORE_INCLUDE_DIRS}
    ${BOTAN_INCLUDE_DIRS}
    ${GIO_INCLUDE_DIRS}
    ${GIO_UNIX_INCLUDE_DIRS}
    ${LIBSOUP_INCLUDE_DIRS}
    ${PROTOBUF_INCLUDE_DIRS})

protobuf_generate_cpp(
    TELEMETRY_PROTOBUF_SOURCES 
    TELEMETRY_PROTOBUF_HEADERS  
    src/airmap/glib/telemetry.proto)

add_library(
  airmap-client SHARED

  include/airmap/aircraft.h
  include/airmap/aircrafts.h
  include/airmap/airspace.h
  include/airmap/airspaces.h
  include/airmap/authenticator.h
  include/airmap/client.h
  include/airmap/date_time.h
  include/airmap/flight.h
  include/airmap/flights.h
  include/airmap/optional.h
  include/airmap/pilot.h
  include/airmap/result.h
  include/airmap/rule.h
  include/airmap/telemetry.h
  include/airmap/timestamp.h

  src/airmap/airspace.cpp
  src/airmap/airspaces.cpp
  src/airmap/client.cpp
  src/airmap/codec.h
  src/airmap/date_time.cpp
  src/airmap/geometry.cpp
  src/airmap/jsend.h
  src/airmap/jsend.cpp
  src/airmap/telemetry.cpp

  src/airmap/glib/client.h
  src/airmap/glib/client.cpp
  src/airmap/glib/aircrafts.h
  src/airmap/glib/aircrafts.cpp
  src/airmap/glib/airspaces.h
  src/airmap/glib/airspaces.cpp
  src/airmap/glib/authenticator.h
  src/airmap/glib/authenticator.cpp
  src/airmap/glib/api.h
  src/airmap/glib/api.cpp
  src/airmap/glib/flights.h
  src/airmap/glib/flights.cpp
  src/airmap/glib/telemetry.h
  src/airmap/glib/telemetry.cpp

  ${TELEMETRY_PROTOBUF_SOURCES}
  ${TELEMETRY_PROTOBUF_HEADERS}
)

add_library(
  airmap-server SHARED

  src/airmap/daemon.h
  src/airmap/daemon.cpp)

add_executable(
    airmap

    src/airmap/airmap.cpp)

add_executable(
    airmapd

    src/airmap/daemon_main.cpp)

set_property(TARGET airmap-client PROPERTY CXX_STANDARD 14)
set_property(TARGET airmap-server PROPERTY CXX_STANDARD 14)
set_property(TARGET airmap PROPERTY CXX_STANDARD 14)
set_property(TARGET airmapd PROPERTY CXX_STANDARD 14)

target_link_libraries(airmap-client ${Boost_LIBRARIES} ${LIBSOUP_LDFLAGS} ${PROTOBUF_LIBRARIES} ${BOTAN_LIBRARIES})
target_link_libraries(airmap airmap-client)
target_link_libraries(airmap-server airmap-client)
target_link_libraries(airmapd airmap-server)
