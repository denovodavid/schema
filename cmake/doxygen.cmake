# https://vicrucann.github.io/tutorials/quick-cmake-doxygen/

# check if Doxygen is installed
find_package(Doxygen)
if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    # build docs
    add_custom_target(doxygen
                      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                      COMMENT "Generating API documentation with Doxygen"
                      VERBATIM)

    # move html folder to docs
    add_custom_command(TARGET doxygen
                       COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_CURRENT_BINARY_DIR}/doxygen-docs/html ${CMAKE_CURRENT_SOURCE_DIR}/docs)
else (DOXYGEN_FOUND)
  message("Doxygen needs to be installed to generate documentation")
endif (DOXYGEN_FOUND)
