cmake_minimum_required(VERSION 3.13)

project(schema VERSION 1.0
               DESCRIPTION "Schema ECS Library"
               LANGUAGES CXX)

add_library(schema STATIC schema.cpp ../include/schema/schema.h)
target_include_directories(schema PUBLIC ../include/schema)
target_compile_features(schema PUBLIC cxx_std_17)
set_target_properties(schema PROPERTIES
                             CXX_STANDARD 17
                             CXX_STANDARD_REQUIRED YES
                             CXX_EXTENSIONS NO)
