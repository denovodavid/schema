# https://arcanis.me/en/2015/10/17/cppcheck-and-clang-format

# additional target to perform clang-format run, requires clang-format

# get source files
file(GLOB_RECURSE ALL_SOURCE_FILES
     "include/*.h"
     "src/*.cpp"
     "sandbox/*.h"
     "sandbox/*.cpp")

add_custom_target(clangformat
                  COMMAND clang-format
                  -style=llvm
                  -i
                  ${ALL_SOURCE_FILES})
