cmake_minimum_required(VERSION 3.5)

project(external-boringssl)

include(FetchContent)

FetchContent_Declare(
    boringssl
    URL file://${CMAKE_CURRENT_LIST_DIR}/boringssl-fips-20180730.tar.gz
    PREFIX boringssl
)

if(ANDROID)
set(no-stdio ON)
if(${ANDROID_ABI} STREQUAL "armeabi-v7a")
    include_directories(${ANDROID_SYSROOT}/usr/include/arm-linux-androideabi)
elseif(${ANDROID_ABI} STREQUAL "arm64-v8a")
    include_directories(${ANDROID_SYSROOT}/usr/include/aarch64-linux-android)
else()
    include_directories(${ANDROID_SYSROOT}/usr/include/arm-linux-androideabi)
endif()
endif()

FetchContent_GetProperties(boringssl)
if(NOT boringssl_POPULATED)
    FetchContent_Populate(boringssl)
    set(OPENSSL_NO_ASM ON)
    set(CMAKE_CXX_FLAGS "-Wno-unused-parameter")
    set(CMAKE_C_FLAGS "-Wno-unused-parameter")
    add_subdirectory(${boringssl_SOURCE_DIR} ${boringssl_BINARY_DIR})
    target_include_directories( ssl INTERFACE ${boringssl_SOURCE_DIR}/include )
endif()
