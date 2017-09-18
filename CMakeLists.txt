cmake_minimum_required(VERSION 3.5)
project(airmap)

set(AIRMAPD_VERSION_MAJOR 0)
set(AIRMAPD_VERSION_MINOR 0)
set(AIRMAPD_VERSION_PATCH 1)

enable_testing()

find_package(Boost REQUIRED date_time filesystem program_options system)
find_package(PkgConfig)
find_package(Protobuf REQUIRED)

pkg_check_modules(OPENSSL REQUIRED openssl)

# Detecting the platform at build time and exposing
# information on the platform to the platform
set(AIRMAP_PLATFORM "null")

if (UNIX)
  # We are treating OS X as a proper Linux-like operating system and store
  # configuration files according to the XDG directory standard.
  set(AIRMAP_PLATFORM "linux")
endif()

message(STATUS "Enabling platform ${AIRMAP_PLATFORM}")

# All vendored dependencies should be listed here
set(SPDLOG_BUILD_TESTING OFF CACHE BOOL "Build the spdlog tests")
set(Uri_BUILD_TESTS OFF CACHE BOOL "Build the URI tests")
set(XDG_BUILD_TESTS OFF CACHE BOOL "Build the xdg tests")

add_subdirectory(vendor/fmt EXCLUDE_FROM_ALL)
add_subdirectory(vendor/spdlog EXCLUDE_FROM_ALL)
add_subdirectory(vendor/uri EXCLUDE_FROM_ALL)
add_subdirectory(vendor/xdg EXCLUDE_FROM_ALL)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fno-strict-aliasing -pedantic -Wextra -fPIC")

include_directories(
  include
  src

  vendor/
  vendor/beast/include
  vendor/fmt
  vendor/mavlink
  vendor/spdlog/include
  vendor/uri/include
  vendor/mqtt_client_cpp/include

  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/src

  ${OPENSSL_INCLUDE_DIRS}
  ${PROTOBUF_INCLUDE_DIRS})

add_subdirectory(src/airmap)
add_subdirectory(test)

find_program(CLANG_FORMAT_EXECUTABLE
  NAMES clang-format clang-format-5.0
        clang-format-4.0 clang-format-3.9
        clang-format-3.8 clang-format-3.7
        clang-format-3.6 clang-format-3.5
        clang-format-3.4 clang-format-3.3
  DOC "clang-format executable"
)

if (CLANG_FORMAT_EXECUTABLE)
  message(STATUS "Enabling format target")
  file(GLOB_RECURSE AIRMAPD_SRCS include/*.h src/*.h src/*.cpp test/*.h test/*.cpp)

  add_custom_target(
    format ${CLANG_FORMAT_EXECUTABLE} -style=file -i ${AIRMAPD_SRCS})
else()
  message(STATUS "Enabling format target")
endif()

set(CPACK_PACKAGE_NAME "airmap")
set(CPACK_PACKAGE_FILE_NAME "airmap-installer")
include (CPack)
