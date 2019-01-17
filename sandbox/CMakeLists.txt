cmake_minimum_required(VERSION 3.13)

project(sandbox VERSION 1.0
                DESCRIPTION "Schema ECS Sandbox"
                LANGUAGES CXX)

add_executable(sandbox sandbox.cpp)
target_link_libraries(sandbox schema)
