cmake_minimum_required(VERSION 3.8)

project(schema VERSION 0.1.0
               DESCRIPTION "Schema ECS Library"
               LANGUAGES CXX)

FILE(GLOB_RECURSE SCHEMA_SRC "*.cpp")
FILE(GLOB_RECURSE SCHEMA_HEADERS "../include/schema/*.h")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}
             PREFIX "src"
             FILES ${SCHEMA_SRC})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/../include/schema"
             PREFIX "include"
             FILES ${SCHEMA_HEADERS})

add_library(schema STATIC ${SCHEMA_SRC} ${SCHEMA_HEADERS})
target_include_directories(schema PUBLIC ../include/schema)
target_compile_features(schema PUBLIC cxx_std_17)
set_target_properties(schema PROPERTIES
                             CXX_STANDARD 17
                             CXX_STANDARD_REQUIRED YES
                             CXX_EXTENSIONS NO)
