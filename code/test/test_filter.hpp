#pragma once

#include <algorithm>
#include <string>
#include "test_desc.hpp"
#include <vector>

namespace rs
{
  // Filters a test using arrays of strings which can be read from the command line.
  struct test_filter
  {
    // exclude tests for a specific object
    std::vector<std::string> object_excludes;

    // include only tests for a specific object
    std::vector<std::string> object_includes;

    // exclude tests with a specific name
    std::vector<std::string> test_excludes;

    // include only tests with a specific name
    std::vector<std::string> test_includes;

    // Creates a test_filter object from the command line.
    static test_filter from_command_line(int argc, char** argv)
    {
      enum { none, objexc, objinc, testexc, testinc };
      int mode = none;
      test_filter filter;

      for (int i = 1; i < argc; ++i)
      {
        std::string arg = argv[i];

        if (arg[0] != '-')
        {
          switch (mode)
          {
          case objexc:
            filter.object_excludes.emplace_back(std::move(arg));
            break;
          case objinc:
            filter.object_includes.emplace_back(std::move(arg));
            break;
          case testexc:
            filter.test_excludes.emplace_back(std::move(arg));
            break;
          case testinc:
            filter.test_includes.emplace_back(std::move(arg));
            break;
          case none:
            break;
          }
        }
        else if (arg.find("objexc") != arg.npos) mode = objexc;
        else if (arg.find("objinc") != arg.npos) mode = objinc;
        else if (arg.find("testexc") != arg.npos) mode = testexc;
        else if (arg.find("testinc") != arg.npos) mode = testinc;
      }

      return std::move(filter);
    }
    
    // Returns a string with usage info for the test_filter object.
    static std::string get_command_line_usage()
    {
      std::string str = "-flag1 filterA filterB -flag2 filterC\n\n";
      str += "  flags:\n";
      str += "    -objexc   exclude tests for a specific object\n";
      str += "    -objinc   include only tests for a specific object\n";
      str += "    -testexc  exclude tests with a specific name\n";
      str += "    -testinc  include only tests with a specific name\n";

      return str;
    }

    // Returns true if a particular test is filtered by this filter.
    bool is_filtered(itest_desc const& test)
    {
      bool filtered;

      // object excludes
      filtered = std::any_of(
        object_excludes.begin(),
        object_excludes.end(),
        [&](std::string const& obj_ex) { return obj_ex == test.get_object_name(); }
      );
      if (filtered) return true;

      // object includes
      filtered = !std::any_of(
        object_includes.begin(),
        object_includes.end(),
        [&](std::string const& obj_inc) { return obj_inc == test.get_object_name(); }
      );
      if (filtered && object_includes.size()) return true;

      // test excludes
      filtered = std::any_of(
        test_excludes.begin(),
        test_excludes.end(),
        [&](std::string const& test_ex) { return test_ex == test.get_test_name(); }
      );
      if (filtered) return true;

      // test includes
      filtered = !std::any_of(
        test_includes.begin(),
        test_includes.end(),
        [&](std::string const& test_inc) { return test_inc == test.get_test_name(); }
      );
      if (filtered && test_includes.size()) return true;

      return false;
    }
  };

} // namespace rs