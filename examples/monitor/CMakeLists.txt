add_executable(
    monitor.client 

    client.cpp)

set_property(TARGET monitor.client PROPERTY CXX_STANDARD 14)

target_link_libraries(
  monitor.client

  airmap-monitor
  airmap-client

  ${Boost_LIBRARIES}
  ${OPENSSL_LDFLAGS}
)