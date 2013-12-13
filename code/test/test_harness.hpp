#pragma once

#include <chrono>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <string>
#include "test_failure.hpp"
#include "test_filter.hpp"
#include <vector>
#ifdef _WIN32
#include <wtypes.h>
#include <WinBase.h>
#endif

namespace rs
{
  // Stores and executes tests.
  class test_harness
  {
  public:
    typedef std::unique_ptr<itest_desc> desc_ptr;
    typedef std::vector<desc_ptr> desc_array;
    typedef std::vector<itest_desc*> pdesc_array;
    typedef std::function<void(std::string const&)> output_function;
    typedef std::pair<bool, test_failure_exception> test_run_result;

  private:
    test_filter m_filter;
    desc_array m_meta_array;
    output_function m_output;
    bool m_output_passed_tests;
    std::chrono::milliseconds m_timeout;

  public:
    // Returns the current test filter.
    static test_filter const& filter()
    {
      return inst().m_filter;
    }

    // Sets the test filter used to run a specified set of tests.
    static void filter(test_filter const& tf)
    {
      inst().m_filter = tf;
    }

    // Returns the current log function used by the test harness.
    static output_function const& log_function()
    {
      return inst().m_output;
    }

    // Set the log function used by the test harness.
    static void log_function(output_function const& log_fn)
    {
      inst().m_output = log_fn;
    }

    // Returns whether test passes are logged.
    static bool log_test_passes()
    {
      return inst().m_output_passed_tests;
    }

    // Flag whether test passes should be logged (false by default).
    static void log_test_passes(bool b)
    {
      inst().m_output_passed_tests = b;
    }

    template <typename descT, std::size_t idx = 0>
    // Registers a test via a test_desc object.
    static void register_test()
    {
      test_harness& harness = inst();
      harness.m_meta_array.emplace_back(new test_desc<descT, idx>());
    }

    // Runs all tests registered with the harness.
    // Returns the number of failed tests.
    static int run()
    {
      test_harness& harness = inst();
      pdesc_array tests;

      // create filtered tests array
      for (desc_ptr& desc : harness.m_meta_array)
        if (!harness.m_filter.is_filtered(*desc))
          tests.emplace_back(desc.get());

      return harness.run_test_array(tests);
    }

    template <typename Rep, typename Period>
    // Sets the cap for the amount of time tests are allowed to run.
    // Individual tests may override this timeout.
    static void timeout(std::chrono::duration<Rep, Period> const& duration)
    {
      inst().m_timeout = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    }

  private:
    // Default timeout is 2 minutes.
    static const std::size_t c_default_timeout = 120 * 1000;

    // Constructs the test_harness object. (private)
    test_harness() : m_timeout(c_default_timeout)
    {
      m_output = [](std::string const& str)
      {
        std::cout << str;
#ifdef _MSC_VER
        OutputDebugStringA(str.c_str());
#endif
      };
    }

    // Performs a test run and handles any failure.
    static test_run_result do_test_run(itest* test)
    {
      try
      {
        test->run();
      }
      catch (test_failure_exception& failure)
      {
        return std::make_pair(true, failure);
      }

      return std::make_pair(false, test_failure_exception());
    }

    // Formats a full test name as object:test.
    static std::string format_test_name(itest_desc const& desc)
    {
      std::string const& obj_name = desc.get_object_name();
      std::string const& test_name = desc.get_test_name();

      std::string str(obj_name);
      if (obj_name.size() && test_name.size()) str += ":";
      str += test_name;
      
      return std::move(str);
    }

    // Returns the test_harness instance.
    static test_harness& inst()
    {
      static test_harness instance;
      return instance;
    }

    // Runs all tests in the given description array
    int run_test_array(std::vector<itest_desc*> const& arr)
    {
      int fail_count = 0;

      // run all tests from their test descriptions
      for (itest_desc* desc : arr)
        if (!run_test_desc(desc))
          fail_count++;

      return fail_count;
    }

    // Runs a test from a test description.
    bool run_test_desc(itest_desc* desc)
    {
      using namespace std;
      using namespace std::chrono;

      milliseconds timeout = desc->get_timeout() == size_t(-1) ? m_timeout : milliseconds(desc->get_timeout());
      unique_ptr<itest> test = move(desc->create());

      try
      {
        // run the test on a separate thread
        future<test_run_result> res = async(launch::async, do_test_run, test.get());
        future_status status = res.wait_for(timeout);

        // if the test has finished:
        if (status == future_status::ready)
        {
          test_run_result run_result = res.get();
          if (run_result.first) throw run_result.second; // test failed
        }
        else // the test has timed out:
        {
          throw test_failure_exception(desc->get_file_name(), desc->get_line_number())
            << "timeout after " << timeout.count() << " milliseconds";
        }
      }
      catch (test_failure_exception& failure)
      {
        // print test failure
        m_output(format_test_name(*desc) + " failed\n  " + failure.what() + "\n");
        return false;
      }

      // print test passed
      if (m_output_passed_tests) m_output(format_test_name(*desc) + " passed\n");
      return true;
    }

  };
} // namespace rs