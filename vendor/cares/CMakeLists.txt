cmake_minimum_required(VERSION 3.5)

include(FetchContent)

FetchContent_Declare(
    cares
    URL https://github.com/c-ares/c-ares/archive/cares-1_13_0.tar.gz
    PREFIX cares
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=$ENV{AIRMAP_CARES_OUTPUT_PATH}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DANDROID_ABI=${ANDROID_ABI}
        -DANDROID_PLATFORM=${ANDROID_PLATFORM}
        -DANDROID_STL=${ANDROID_STL}
        -DANDROID_CPP_FEATURES=${ANDROID_CPP_FEATURES}
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DCARES_STATIC=ON
        -DCARES_SHARED=OFF
        -DCARES_STATIC_PIC=ON
    CMAKE_GENERATOR ${CMAKE_GENERATOR}
    UPDATE_COMMAND ""
)

FetchContent_GetProperties(cares)
if(NOT cares_POPULATED)
    FetchContent_Populate(cares)
    add_subdirectory(${cares_SOURCE_DIR} ${cares_BINARY_DIR})
endif()