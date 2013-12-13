#include <fstream>
#include "test/test.hpp"

// file_io test function
deftest(file_io)
{
  std::ofstream ofile("test_file.txt");
  testassert(ofile.is_open()) << "Output file could not be opened";

  ofile << "Avoid using file I/O in tests." << std::endl;
  ofile << "The success of your test may be bound to OS filesystem conditions." << std::endl;
  ofile.close();

  std::ifstream ifile("test_file.txt");
  testassert(ifile.is_open()) << "Input file could not be opened";

  //std::ifstream ifile2("test_fileoops.txt");
  //testassert(ifile2.is_open()) << "Input file could not be opened";

  std::remove("test_file.txt");
}

// description of the file_io test
desctest(file_io)
{
  objectname = "fstream"; // name of the class we're testing
  testname = "file_io";   // name of the individual test
  timeout = 1000;         // override the test_harness' timeout value
}

// Equivalent to above; deftest_basic wraps both the test function and test description.
deftest_basic(fstream, file_io)
{
  std::ofstream ofile("test_file.txt");
  testassert(ofile.is_open()) << "Output file could not be opened";

  ofile << "Avoid using file I/O in tests." << std::endl;
  ofile << "The success of your test may be bound to OS filesystem conditions." << std::endl;
  ofile.close();

  std::ifstream ifile("test_file.txt");
  testassert(ifile.is_open()) << "Input file could not be opened";

  //std::ifstream ifile2("test_fileoops.txt");
  //testassert(ifile2.is_open()) << "Input file could not be opened";

  std::remove("test_file.txt");
}
