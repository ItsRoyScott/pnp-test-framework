#pragma once

#include <memory>
#include <string>

namespace rs
{
  struct itest;
  struct itest_desc;
  class test_harness;

  struct itest
  {
    virtual ~itest() {}
    virtual void run() = 0;
  };

  struct itest_desc
  {
    virtual ~itest_desc() {}
    virtual std::unique_ptr<itest> create() const = 0;
    virtual char const* get_file_name() const = 0;
    virtual int get_line_number() const = 0;
    virtual std::string const& get_object_name() const = 0;
    virtual std::string const& get_test_name() const = 0;
    virtual std::size_t get_timeout() const = 0;
  };

} // namespace rs

template <typename T, std::size_t> struct test_desc;