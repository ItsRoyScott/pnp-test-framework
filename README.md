pnp-test-framework
==================

Simple header-only test framework.

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

## Exercise: Add Setup/Teardown Functions

Given the structure of having an `itest` separate from its `itest_desc` description object allows us to implement setup and teardown functions easily. The process would look like this:

1. Add getters in `itest_desc` for setup/teardown functions.
2. Add setup/teardown functions to the fields in the `base_test_desc` class.
3. Add code in test_harness to call `setup` prior to `run` and `teardown` after `run`.

Then you can assign setup and teardown functions from within a `desctest(test_t)` function.
