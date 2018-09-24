cmake_minimum_required(VERSION 3.5)

# This cmake setup builds boost in a way suitable for linking with
# the AirMap platform SDK. 
#
# We are aiming to support the following platforms/architectures:
#
#   * linux/windows/android/macOS x86(64)
#   * linux/android/ios arm(64)
#
# To this end, we translate CMake variables and cross-compilation facilities
# to boost.build.
include(FetchContent)

set(
  AIRMAP_BOOST_COMPONENTS
  "--with-date_time --with-filesystem --with-log --with-program_options --with-test --with-system --with-thread"
)

if(${ANDROID})
  if (${ANDROID_ABI} STREQUAL "armeabi")
    set(AIRMAP_BOOST_B2_COMMAND_FLAGS_ARCHITECTURE architecture=arm)
  elseif (${ANDROID_ABI} STREQUAL "armeabi-v7a")
    set(AIRMAP_BOOST_B2_COMMAND_FLAGS_ARCHITECTURE architecture=arm)
  elseif (${ANDROID_ABI} STREQUAL "arm64-v8a")
    set(AIRMAP_BOOST_B2_COMMAND_FLAGS_ARCHITECTURE architecture=arm)
  elseif (${ANDROID_ABI} STREQUAL "x86")
    set(AIRMAP_BOOST_B2_COMMAND_FLAGS_ARCHITECTURE architecture=x86)
  elseif (${ANDROID_ABI} STREQUAL "x86_64")
    set(AIRMAP_BOOST_B2_COMMAND_FLAGS_ARCHITECTURE architecture=ia64)
  endif ()
else ()
  set(AIRMAP_BOOST_CXX_FLAGS "cxxflags=-fPIC")
endif ()


set(BOOST_URL https://dl.bintray.com/boostorg/release/1.68.0/source/boost_1_68_0.tar.bz2)
#    set(BOOST_URL_SHA256 5721818253e6a0989583192f96782c4a98eb6204965316df9f5ad75819225ca9)
set(BOOST_URL_SHA256 7f6130bc3cf65f56a618888ce9d5ea704fa10b462be126ad053e80e553d6d8b7)
FetchContent_Declare(boost
    GIT_REPOSITORY https://github.com/Orphis/boost-cmake.git
    BUILD_IN_SOURCE 0
)

set(BOOST_THREAD_USES_DATETIME 1)
set(BOOST_STANDALONE OFF)
set(BOOST_LOG_WITHOUT_IPC OFF)
set(BOOST_ASIO_HAS_STD_STRING_VIEW 1)
FetchContent_GetProperties(boost)
if(NOT boost_POPULATED)
    FetchContent_Populate(boost)
    add_subdirectory(${boost_SOURCE_DIR} ${boost_BINARY_DIR})
    set_target_properties( Boost::boost PROPERTIES INTERFACE_COMPILE_DEFINITIONS BOOST_THREAD_USES_DATETIME )
    set_target_properties( Boost::boost PROPERTIES INTERFACE_COMPILE_DEFINITIONS BOOST_ASIO_HAS_STD_STRING_VIEW )
endif()
