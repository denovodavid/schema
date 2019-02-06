cmake_minimum_required(VERSION 3.8)

project(sandbox DESCRIPTION "Schema ECS Sandbox"
                LANGUAGES CXX)

FILE(GLOB SANDBOX_SRC "sandbox.cpp")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}
             FILES ${SANDBOX_SRC})

add_executable(sandbox ${SANDBOX_SRC})
target_link_libraries(sandbox schema)
