add_library(
  airmap-cpp SHARED

  $<TARGET_OBJECTS:airmap-client>
)

set_target_properties(
  airmap-cpp PROPERTIES

  CXX_STANDARD      14
  SOVERSION         "${AIRMAPD_VERSION_MAJOR}.${AIRMAPD_VERSION_MINOR}.${AIRMAPD_VERSION_PATCH}"
  VERSION           "${AIRMAPD_VERSION_MAJOR}"
)

if (AIRMAP_ENABLE_GRPC)
  list(
    APPEND CONDITIONAL_LIBRARIES
    airmap-codec-grpc
    airmap-grpc
    airmap-monitor
  )
endif ()

target_link_libraries(
  airmap-cpp
  airmap-mavlink

  ${CONDITIONAL_LIBRARIES}

  Boost::date_time
  Boost::filesystem
  Boost::log
  Boost::program_options
  Boost::system
  Boost::thread

  crypto
  ssl
  decrepit

  libprotobuf
)

install(
  TARGETS airmap-cpp
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)