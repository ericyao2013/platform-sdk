cmake_minimum_required(VERSION 3.5)

project(external-boringssl)

include(FetchContent)
include(ExternalProject)

FetchContent_Declare(
    boringssl
    URL https://boringssl.googlesource.com/boringssl/+archive/66005f41fbc3529ffe8d007708756720529da20d.tar.gz
    PREFIX boringssl
)

FetchContent_GetProperties(boringssl)
if(NOT boringssl_POPULATED)
    FetchContent_Populate(boringssl)
    set(OPENSSL_NO_ASM=ON)
    add_subdirectory(${boringssl_SOURCE_DIR} ${boringssl_BINARY_DIR})
    target_include_directories( ssl INTERFACE ${boringssl_SOURCE_DIR}/include ${boringssl_SOURCE_DIR}/include/openssl)
endif()