#pragma once

#include "test_harness.hpp"

namespace rs
{
  template <typename T, std::size_t index = 0>
  // Automatically registers a test with the test harness at startup.
  struct auto_add_test
  {
    auto_add_test() { rs::test_harness::register_test<T, index>(); }
    static auto_add_test inst;
  };
  template <typename T, std::size_t index>
  auto_add_test<T, index> auto_add_test<T, index>::inst;

  template <typename T, std::size_t index = 0>
  // Base class for test descriptions which provide test creation and metadata info. 
  class base_test_desc : public itest_desc
  {
  public:
    base_test_desc(char const* file, int line) :
      filename(file),
      linenumber(line),
      timeout(std::size_t(-1))
    {}
    ~base_test_desc() { (void) s_autoadd; }

    std::unique_ptr<itest> create() const { return std::unique_ptr<T>(new T()); }
    char const* get_file_name() const { return filename; }
    int get_line_number() const { return linenumber; }
    std::string const& get_object_name() const override { return objectname; }
    std::string const& get_test_name() const override { return testname; }
    std::size_t get_timeout() const override { return timeout; }

  protected:
    char const* filename;
    int linenumber;
    std::string objectname;
    std::string testname;
    std::size_t timeout;

  private:
    static auto_add_test<T, index> s_autoadd;
  };
  template <typename T, std::size_t index>
  auto_add_test<T, index> base_test_desc<T, index>::s_autoadd;
} // namespace rs

template <typename T, std::size_t counter = 0>
struct test_desc : public rs::base_test_desc<T>{};

// Defines a test function. Follow with function curly braces: {}.
#define deftest(test_t) struct test_##test_t : public rs::itest { void run() override; }; void test_##test_t::run()

// Defines a test function as well as a description. Follow with function curly braces: {}.
#define deftest_basic(object_name, test_name) \
  struct object_name##test_name##_test : public rs::itest \
  { \
    void run() override; \
  }; \
  template <> struct test_desc<object_name##test_name##_test, __LINE__> : public rs::base_test_desc<object_name##test_name##_test, __LINE__> \
  { \
    test_desc() : base_test_desc(__FILE__, __LINE__) \
    { \
      objectname = #object_name; \
      testname = #test_name; \
    } \
  }; \
  void object_name##test_name##_test::run()

// Defines a test class, allowing you to store variables used for the test.
// Follow with class curly braces and semicolon: {};.
// Add fields you'd like to the class and add a function with the signature
//   `void run() override` to execute test code.
#define deftest_class(test_t) struct test_##test_t : public rs::itest

// Describes a test function. Follow with function curly braces: {}.
// Flags:
//    objectname  sets the object name for the test (string)
//    testname    sets the name of the test (string)
//    timeout     override the test_harness' timeout value for this test
#define desctest(test_t) \
    template <> struct test_desc<test_##test_t, __LINE__> : public rs::base_test_desc<test_##test_t, __LINE__> \
    { \
      test_desc(); \
    }; \
    test_desc<test_##test_t, __LINE__>::test_desc() : base_test_desc(__FILE__, __LINE__)
