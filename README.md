pnp-test-framework
==================

Simple header-only test framework in C++11.

## Features

* Easy to add and run tests: <br>
  1) Add a cpp file to your project. <br>
  2) In it, create a test function: `deftest_basic([object to test], [test name]) {}` <br>
  3) In your `main` function, call `rs::test_harness::run`. <br>
* Filters to limit the total number of tests, or run a specific test.
* Test failure exception provides << insertion operator to append info about the failure.

Note that this framework is missing some significant features you'd expect from a full-fledged testing framework, such as setups and teardowns. However, this is a good starting point if you want to roll your own, and said features wouldn't be difficult to implement.

## Simple Test

```cpp
deftest_basic(fstream, file_io)
{
  std::ofstream ofile("test_file.txt");
  testassert(ofile.is_open()) << "Output file could not be opened";

  ofile << "Avoid using file I/O in tests." << std::endl;
  ofile << "The success of your test may be bound to OS filesystem conditions." << std::endl;
  ofile.close();

  std::ifstream ifile("test_file.txt");
  testassert(ifile.is_open()) << "Input file could not be opened";

  std::remove("test_file.txt");
}
```

The test is automatically registered with the test harness and executed when you call `test_harness::run`.

## Using in Your Project

1. [Download](https://github.com/roskittle/pnp-test-framework/archive/master.zip) the repo to your machine.
2. Copy the `test` folder (from within `code`) into your project.
3. Add the path containing the new `test` folder to the include directories for your build.
4. In your code `#include "test/test.hpp"`.

## Compiling the Repo

On Windows: <br>
1. In the `project` folder, run either `gmake.bat` or `vs2013.bat`.<br>
2. This will generate a Makefile or Visual Studio solution you can use to build the project.<br>

On other platforms:<br>
1. Clone the [premake-dev repo](https://bitbucket.org/premake/premake-dev) and compile.<br>
2. Run the resulting executable file in the `code` folder with the command `./premake5 gmake`<br>
3. This will generate a Makefile in the `project` folder.<br>

## Exercise: Add Setup/Teardown Functions

Given the structure of having an `itest` separate from its `itest_desc` description object allows us to implement setup and teardown functions easily. The process would look like this:

1. Add getters in `itest_desc` for setup/teardown functions.
2. Add setup/teardown functions to the fields in the `base_test_desc` class.
3. Add code in `test_harness` to call `setup` prior to `run` and `teardown` after `run`.

Then you can assign setup and teardown functions from within a `desctest(test_t)` function.
