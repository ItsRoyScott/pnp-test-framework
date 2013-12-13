#include "test/test.hpp"

using namespace rs;
using namespace std;

int main(int argc, char* argv[])
{
  if (argc >= 2)
  {
    string flag = argv[1];

    if (flag == "-h" || flag == "-?" || flag == "--help" || flag == "?" || flag == "/?")
    {
      cout << "usage: " << test_filter::get_command_line_usage() << endl;
      return 0;
    }
  }

  test_harness::filter(test_filter::from_command_line(argc, argv));
  test_harness::log_test_passes(true);

  return test_harness::run();
}