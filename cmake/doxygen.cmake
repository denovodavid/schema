# https://vicrucann.github.io/tutorials/quick-cmake-doxygen/
# https://stackoverflow.com/questions/18590445/cmake-custom-command-to-copy-and-rename

# set docs directories
set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doxygen-docs)
set(HTML_DOCS_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs)

# set input and output doxyfiles
set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

# check if Doxygen is installed
find_package(Doxygen)
if (DOXYGEN_FOUND)
    # request to configure the file
    configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

    # clean folder and build docs
    add_custom_target(doxygen
                      COMMAND ${CMAKE_COMMAND} -E remove_directory ${DOXYGEN_OUTPUT_DIRECTORY}
                      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
                      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                      COMMENT "Generating API documentation with Doxygen"
                      VERBATIM)

    # move html folder to docs
    add_custom_command(TARGET doxygen
                       COMMAND ${CMAKE_COMMAND} -E copy_directory ${DOXYGEN_OUTPUT_DIRECTORY}/html ${HTML_DOCS_OUTPUT_DIRECTORY})

else (DOXYGEN_FOUND)
  message("Doxygen needs to be installed to generate documentation")
endif (DOXYGEN_FOUND)
