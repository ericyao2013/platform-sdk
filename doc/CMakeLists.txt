find_program(DOXYGEN_EXECUTABLE
  NAMES doxygen
  DOC "doxygen executable")

  if (DOXYGEN_EXECUTABLE)
    message(STATUS "Enabling make target 'doc'")
    configure_file(Doxyfile.in Doxyfile @ONLY)
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
  else()
    message(STATUS "Could not find doxygen executable")
  endif()
