# https://cliutils.gitlab.io/modern-cmake/
# https://github.com/ttroy50/cmake-examples
# https://www.youtube.com/playlist?list=PLK6MXr8gasrGmIiSuVQXpfFuE1uPT615s

cmake_minimum_required(VERSION 3.8)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(SchemaECS)

add_subdirectory(src)
add_subdirectory(sandbox)

include(cmake/clang-format.cmake)
include(cmake/doxygen.cmake)
