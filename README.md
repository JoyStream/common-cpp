
# Common

Common crypto C++ library building on [CoinCore](https://github.com/JoyStream/coincore-cpp)

conan package name: `Common/0.2.0@joystream/stable`

### Dependencies

This library, has *immediate* dependencies

- CoinCore

and they are managed using [Conan](https://conan.io), a platform and build system agnostic C++ package manager.

### Example Usage

When working with a local copy of the sources, the simplest way is to export the package to the testing channel, `Common/0.0.0@joystream/testing`. Inside the `conan_package/` directory, run the test_package command, this will export and do a test build of the package:

```
conan test_package
```

Next we need to install the library and it's dependencies into our consuming project. Move into the `example/` directory and run:

```
conan install ./ --build=missing
```

This will produce an appropriate `conanbuildinfo.cmake` file, used in the building of the library itself, which is used in the example
project CMakeLists.txt to find the library. Now we can build our project:

```
cmake .
cmake --build .
```

And finally run the built example:
```
bin/example
```

## Running Unit Tests

```
sh run_tests.sh
```

## License

JoyStream Common library is released under the terms of the MIT license.
See [LICENSE](LICENCE) for more information.
