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
  date_time filesystem log program_options test system thread
)

function(add_boost_libs)
  foreach(COMPONENT ${AIRMAP_BOOST_COMPONENTS})
   if(NOT (TARGET Boost_${COMPONENT} ) )
	message("Creating Boost_${COMPONENT}")
  	add_library( Boost_${COMPONENT} STATIC IMPORTED GLOBAL )
	add_dependencies( Boost_${COMPONENT} boost-build )
	set_target_properties( Boost_${COMPONENT} PROPERTIES
			IMPORTED_LOCATION ${boost_BINARY_DIR}/lib/libboost_${COMPONENT}.a
			INTERFACE_INCLUDE_DIRECTORIES ${boost_BINARY_DIR}/include
			INTERFACE_COMPILE_DEFINITIONS BOOST_THREAD_USES_DATETIME
			)
  	if(${ANDROID})
		target_compile_definitions( Boost_${COMPONENT} INTERFACE BOOST_ASIO_HAS_STD_STRING_VIEW )
  	endif()
    endif()
  endforeach()
endfunction()

function(PREFIX_AND_SUFFIX var prefix suffix)
  set(listVar "")
  foreach(f ${ARGN})
	LIST(APPEND listVar ${prefix}${f}${suffix})
  endforeach()
  set(${var} ${listVar} PARENT_SCOPE)
endfunction()

if (${ANDROID})
  message(STATUS "Enabling Android build for Boost - ${ANDROID_ABI}")

  string(REPLACE " " " <cflags>"    AIRMAP_BOOST_ANDROID_STANDARD_C_FLAGS       "<cflags>${CMAKE_C_FLAGS}")
  string(REPLACE " " " <cxxflags>"  AIRMAP_BOOST_ANDROID_STANDARD_CXX_FLAGS     "<cxxflags>${CMAKE_CXX_FLAGS}")
  string(REPLACE " " " <linkflags>" AIRMAP_BOOST_ANDROID_STANDARD_CXX_LIBRARIES "<linkflags>${CMAKE_CXX_STANDARD_LIBRARIES}")

  foreach (isystem IN LISTS CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES)
    set(AIRMAP_BOOST_ANDROID_STANDARD_INCLUDE_DIRECTORIES "<cxxflags>-isystem <cxxflags>${isystem} ${AIRMAP_BOOST_ANDROID_STANDARD_INCLUDE_DIRECTORIES}")
  endforeach ()

  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/android/user-config.jam.in
    ${CMAKE_CURRENT_BINARY_DIR}/android/user-config.jam
  )

  set(AIRMAP_BOOST_B2_COMMAND_FLAGS_USER_CONFIG --user-config=${CMAKE_CURRENT_BINARY_DIR}/android/user-config.jam)
  set(AIRMAP_BOOST_B2_COMMAND_FLAGS_TARGET_OS target-os=android)
  set(AIRMAP_BOOST_B2_COMMAND_FLAGS_TOOLSET toolset=clang-android)

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

if (AIRMAP_BOOST_B2_COMMAND_FLAGS_USER_CONFIG)
  message(STATUS "  user config: ${AIRMAP_BOOST_B2_COMMAND_FLAGS_USER_CONFIG}")
else ()
  message(STATUS "  user config: not set")
endif()

if (AIRMAP_BOOST_B2_COMMAND_FLAGS_ARCHITECTURE)
  message(STATUS "  arch:        ${AIRMAP_BOOST_B2_COMMAND_FLAGS_ARCHITECTURE}")
else ()
  message(STATUS "  arch:        not set")
endif ()

if (AIRMAP_BOOST_B2_COMMAND_FLAGS_TOOLSET)
  message(STATUS "  toolset:     ${AIRMAP_BOOST_B2_COMMAND_FLAGS_TOOLSET}")
else ()
  message(STATUS "  toolset:     not set")
endif ()

if(UNIX)
  set(AIRMAP_BOOST_BOOTSTRAP_COMMAND ./bootstrap.sh)
  set(AIRMAP_BOOST_B2_COMMAND ./b2)
else()
  if(WIN32)
    set(AIRMAP_BOOST_BOOTSTRAP_COMMAND bootstrap.bat)
    set(AIRMAP_BOOST_B2_COMMAND b2.exe)
  endif()
endif()

FetchContent_Declare(boost
    URL https://dl.bintray.com/boostorg/release/1.68.0/source/boost_1_68_0.tar.bz2
    URL_HASH SHA256=7f6130bc3cf65f56a618888ce9d5ea704fa10b462be126ad053e80e553d6d8b7
    BUILD_IN_SOURCE 0
)

set(BOOST_THREAD_USES_DATETIME 1)
set(BOOST_STANDALONE OFF)
set(BOOST_LOG_WITHOUT_IPC OFF)
if(${ANDROID})
  set(BOOST_ASIO_HAS_STD_STRING_VIEW 1)
endif()
FetchContent_GetProperties(boost)
if(NOT boost_POPULATED)
  FetchContent_Populate(boost)
  PREFIX_AND_SUFFIX(boost_components --with- "" ${AIRMAP_BOOST_COMPONENTS})
  PREFIX_AND_SUFFIX(boost_libs ${boost_BINARY_DIR}/lib/libboost_ .a ${AIRMAP_BOOST_COMPONENTS})
  message("configuring boost")
  execute_process(
	COMMAND ${AIRMAP_BOOST_BOOTSTRAP_COMMAND}
	WORKING_DIRECTORY ${boost_SOURCE_DIR}
	RESULT_VARIABLE errno
	ERROR_VARIABLE err)

  if(NOT ${errno} EQUAL 0)
	message(ERROR "Failed building boost. error: ${err}")
  endif()

  message("building boost")
  execute_process(
      COMMAND ${AIRMAP_BOOST_B2_COMMAND} ${AIRMAP_BOOST_B2_COMMAND_FLAGS_USER_CONFIG}
        ${boost_components}
        --disable-icu
        --prefix=${boost_BINARY_DIR}
        define=BOOST_LOG_WITHOUT_IPC
        cxxflags=-fvisibility=hidden
        cxxflags=-fvisibility-inlines-hidden
        ${AIRMAP_BOOST_CXX_FLAGS}
        ${AIRMAP_BOOST_B2_COMMAND_FLAGS_TOOLSET}
        ${AIRMAP_BOOST_B2_COMMAND_FLAGS_ARCHITECTURE}
        ${AIRMAP_BOOST_B2_COMMAND_FLAGS_TARGET_OS}
        threading=multi link=static variant=release install
       WORKING_DIRECTORY ${boost_SOURCE_DIR}
       RESULT_VARIABLE errno
       ERROR_VARIABLE err)

  if(NOT ${errno} EQUAL 0)
	message(ERROR "Failed building boost. error: ${err}")
  endif()
endif()
add_boost_libs()
