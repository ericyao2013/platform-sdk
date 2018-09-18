cmake_minimum_required(VERSION 3.5)

include(FetchContent)

list(
    APPEND CMAKE_FIND_ROOT_PATH
    $ENV{AIRMAP_EXTERNAL_DEPENDENCIES_OUTPUT_PATH}
)

FetchContent_Declare(
    grpc
    GIT_REPOSITORY https://github.com/airmap/grpc.git
    PREFIX grpc
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=$ENV{AIRMAP_GRPC_OUTPUT_PATH}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DANDROID_ABI=${ANDROID_ABI}
        -DANDROID_PLATFORM=${ANDROID_PLATFORM}
        -DANDROID_STL=${ANDROID_STL}
        -DANDROID_CPP_FEATURES=${ANDROID_CPP_FEATURES}
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DCMAKE_FIND_ROOT_PATH:PATH=${CMAKE_FIND_ROOT_PATH}
        -DCMAKE_PREFIX_PATH:PATH=$ENV{AIRMAP_EXTERNAL_DEPENDENCIES_OUTPUT_PATH}
        -DgRPC_BUILD_TESTS=OFF
        -DgRPC_ZLIB_PROVIDER=package
        -DgRPC_CARES_PROVIDER=package
        -DgRPC_SSL_PROVIDER=package
        -DOPENSSL_ROOT_DIR=${OPENSSL_ROOT_DIR}
        -DOPENSSL_USE_STATIC_LIBS=${OPENSSL_USE_STATIC_LIBS}
        -DgRPC_PROTOBUF_PROVIDER=package
        -DgRPC_PROTOBUF_PACKAGE_TYPE=CONFIG
        -DgRPC_INSTALL_default=ON
    CMAKE_GENERATOR ${CMAKE_GENERATOR}
    UPDATE_COMMAND ""
)
#ExternalProject_add_step( grpc touchFile
#    COMMAND ${CMAKE_COMMAND} -E make_directory $ENV{AIRMAP_GRPC_OUTPUT_PATH}/lib/cmake/grpc/
#    COMMAND ${CMAKE_COMMAND} -E touch file $ENV{AIRMAP_GRPC_OUTPUT_PATH}/lib/cmake/grpc/gRPCTargets.cmake
#    DEPENDEES build
#)


FetchContent_GetProperties(grpc)
if(NOT grpc_POPULATED)
    FetchContent_Populate(grpc)
    add_subdirectory(${grpc_SOURCE_DIR} ${grpc_BINARY_DIR})
endif()