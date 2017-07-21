cmake_minimum_required(VERSION 3.5)
project(airmap)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fno-strict-aliasing -pedantic -Wextra -fPIC")

find_package(Boost REQUIRED date_time program_options system)
find_package(PkgConfig)
find_package(Protobuf REQUIRED)

pkg_check_modules(LIBSOUP REQUIRED libsoup-2.4)
pkg_check_modules(GIO gio-2.0)
pkg_check_modules(GIO_UNIX gio-unix-2.0)

# cryptopp lacks a pkgconfig setup on OS X and 
# we will likely have to adjust the brew formula
set(CRYPTOPP_LIBRARIES cryptopp)

include_directories(
    include
    src

    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/src

    ${AWS_CPP_SDK_CORE_INCLUDE_DIRS}
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
  include/airmap/outcome.h
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

  src/airmap/util/cli.h
  src/airmap/util/cli.cpp
  
  ${TELEMETRY_PROTOBUF_SOURCES}
  ${TELEMETRY_PROTOBUF_HEADERS}
)

add_library(
  airmap-server SHARED

  src/airmap/daemon.h
  src/airmap/daemon.cpp)

add_executable(
    airmap

    src/airmap/cmds/airmap/airmap.cpp)

add_executable(
    airmapd

    src/airmap/cmds/airmapd/airmapd.cpp)

set_property(TARGET airmap-client PROPERTY CXX_STANDARD 14)
set_property(TARGET airmap-server PROPERTY CXX_STANDARD 14)
set_property(TARGET airmap PROPERTY CXX_STANDARD 14)
set_property(TARGET airmapd PROPERTY CXX_STANDARD 14)

target_link_libraries(airmap-client ${Boost_LIBRARIES} ${CRYPTOPP_LIBRARIES} ${LIBSOUP_LDFLAGS} ${PROTOBUF_LIBRARIES})
target_link_libraries(airmap airmap-client )
target_link_libraries(airmap-server airmap-client)
target_link_libraries(airmapd airmap-server)
