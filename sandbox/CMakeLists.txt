cmake_minimum_required(VERSION 3.8)

project(sandbox DESCRIPTION "Schema ECS Sandbox"
                LANGUAGES CXX)

add_executable(sandbox sandbox.cpp)
target_link_libraries(sandbox schema)
