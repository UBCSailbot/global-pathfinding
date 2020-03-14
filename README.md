# Global Pathfinding

A pathfinding solution that will find the fastest and safest way for a sailboat to get between two locations on earth. When finished, this program will return a path based on the following parameters:

- Wind angle and magnitude (Downloaded from PredictWind)
- Current GPS position and goal coordinates
- Land and other no-go zones
- Waves? (Also PredictWind)

## Setup

### Initial Setup
Before you can build the project, you first need to install the dependencies.


#### Option 1 (Recommended)
For UNIX, a handy install script is provided at the top level.
Simply run `./configure`, and dependencies will be installed and the project will be built.

This works for both macOS and Debian users.

#### macOS

1.  [Buy a Mac](http://store.apple.com)
2.  `./install_deps/install_deps_osx.sh`

Depending on your env you may run into permission issues. Here's how to fix them:
```bash
sudo chown -R `whoami` /usr/local
```

#### Debian Linux (Ubuntu)
```bash
./install_deps/install_deps_linux.sh
```

#### Windows
Not supported at this time. Feel free to create a PR with instructions on how to get it working.
Go to the CMake Website: http://www.cmake.org/download/

## Building

If you do not want to use the configure script:
1. Question yourself.
2. Run `mkdir build` in the repo root directory, which will create a folder called `build`.
3. Navigate into the folder using `cd build`.
4. Then run `cmake ..`, which will tell `CMake` to build a project out of the repo directory into your `build` directory.

## Style Guide
Conforming to a common style guide helps us iterate quickly and avoid creating bugs.

We follow [Google's C++ Style Guide](https://google.github.io/styleguide/cppguide.html).
Please familiarize yourself with it.

If you see something that doesn't conform to it, submit a PR.

## Linting
Linting helps us keep our code clean, easy to maintain, and free of bugs.

You can run the cpp lint script from our friends at Google with:

```bash
./scripts/run_cpplint.sh
```

## Running Program
To run the program from our launch site to Hawaii, simply run
```bash
./build/bin/pathfinder_cli -p 10 --navigate 48 235 20 206
```
This should generate a path based on current weather conditions and will produce 2 KML files:
-`build/bin/Path.kml`, a visualization of the path generated, based on the GPS coordinates of the path
-`build/bin/Wind.kml`, a visualization of wind data over time (each color represents a 3 hour increment, based on the estimated speed of the boat)

To see these files, navigate to [Google Earth](https://earth.google.com) and from the menu on the side, select `Projects>New Project>Import KML file from computer`

The program will also generate a string consisting of the [Longitude, Latitude] of each GPS waypoint. Additionally, there is an unused variable of type vector<pair<double,double>> in `./src/pathfinding/PathfinderResultPrinter.cpp` that contains the GPS data in the same format.

### Adjusting Cost Function

Finally, it is possible to adjust the weight given to wind measurements in the pathfinding cost function by navigating to `src/pathfinding/WeatherCostCalculator.cpp` and changing the value of WEATHER_FACTOR. Currently, 10000 seems to be fairly optimal in terms of optimal routing vs. compile time.

When you are done, run
```bash
./build/make
```
and run the program again, as outlined above.

### Testing

##### Creating & Running Tests
Whenever you add new tests, you will need to add the required `.cpp` and `.h` files to the `TEST_FILES` parameter in `test/basic_tests/CMakelists.txt`.

For example:
```cmake
set(TEST_FILES
    example_tests/NewTest.cpp
    example_tests/NewTest.h)
```

To run the tests, use CMake to compile the project, then run `basic_tests` in `build/bin`.
It will notify you if tests pass or fail.

If your tests fail, it will make the build fail which will make people upset.

## Structure of the code
The directories `src` and `test/basic_tests` should mirror each other. That is, any unit testing code for the file `src/a/b.cpp` should be in `test/basic_tests/a/bTest.cpp`.

-   **src/** - Source code. For more details, check out the README files in each sub-directory.
    -   **core/** - The foundation of the source code. We avoid putting target or device-specific code here if we can, as it makes it easier to compile the fundamentals.
    -   **main/** - All the files that contain binary targets.
-   **test/** - Testing, including both unit tests and manually-run tests.
    -   **lib/** - Libraries that are required for testing.
    -   Refer to documentation on **src/** for a guide to the other sub-directories here.
-   **lib/** - Libraries that are required for the core binaries.

## If you have problems
 - Check that you've initiated the git submodules.
 - Check that all dependencies are met.
 - Re-run `cmake ..`
 - If all else fails, delete your build directory and try again.

## Contributing
Please read the [contribution guide](CONTRIBUTING.md).
