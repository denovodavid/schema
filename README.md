# Schema ECS

> C++ Entity Component System

## Guide :poop:

The project uses CMake.
It comprises of two sub projects:
- schema
- sandbox

**schema** is the ECS library :book:  
**sandbox** is the playground :tada:

*If you know how CMake works, do whatever you want.*

This is help:
https://cliutils.gitlab.io/modern-cmake/

Check your IDE for CMake support.  
I use **Xcode** with `cmake -S . -B ./build -G Xcode`  
And, [Visual Studio Code](https://code.visualstudio.com/) with [CMake Tools](https://marketplace.visualstudio.com/items?itemName=vector-of-bool.cmake-tools).

But, now for some basic unix terminal usage:

```sh
# configures le project
cmake -S . -B ./build

# builds le project
make -C ./build

# runs le sandbox executable
./build/sandbox/sandbox
```

### Code Style is [LLVM Style](https://llvm.org/docs/CodingStandards.html) (mostly)

```sh
# format file/s to llvm (default) style
# run the clangformat CMake target
# OR
clang-format -style=llvm -i [<file> ...]
```

## License

[MIT](http://opensource.org/licenses/MIT)
