cmake_minimum_required(VERSION 3.5)

include(FetchContent)

if (${CMAKE_CROSSCOMPILING})
  set(AIRMAP_PROTOBUF_BUILD_PROTOC_BINARIES OFF)
else ()
  set(AIRMAP_PROTOBUF_BUILD_PROTOC_BINARIES ON)
endif ()

FetchContent_Declare(
    protobuf
    URL https://github.com/google/protobuf/releases/download/v3.6.1/protobuf-cpp-3.6.1.tar.gz
    PREFIX protobuf
    PATCH_COMMAND patch -p1 < ${CMAKE_CURRENT_LIST_DIR}/patches/fix-linking-for-android.patch
)

FetchContent_GetProperties(protobuf)
if(NOT protobuf_POPULATED)
    FetchContent_Populate(protobuf)
    add_subdirectory(${protobuf_SOURCE_DIR}/cmake ${protobuf_BINARY_DIR})
endif()