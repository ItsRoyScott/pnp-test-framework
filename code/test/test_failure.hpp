#pragma once

#include <exception>
#include <sstream>

namespace rs
{
  // Exception thrown when a test fails.
  class test_failure_exception : public std::exception
  {
  private:
    std::stringstream m_failure_info;
    std::string m_what;

  public:
    test_failure_exception() {}

    test_failure_exception(const char* file, int line) 
    {
      m_failure_info << file << "(" << line << "): ";
    }

    test_failure_exception(const test_failure_exception& other) 
    {
      m_failure_info << other.m_failure_info.str();
    }

    test_failure_exception& operator=(const test_failure_exception& b)
    {
      m_failure_info.clear();
      m_failure_info << b.m_failure_info.str();
      return *this;
    }

#ifdef _MSC_VER
    const char* what() const override
#endif
#ifdef __GNUG__
    const char* what() const noexcept(true) override
#endif
    {
      const_cast<std::string&>(m_what) = m_failure_info.str();
      return m_what.c_str();
    }

    template <typename T>
    const test_failure_exception& operator<<(T&& x) const 
    {
      const_cast<std::stringstream&>(m_failure_info) << std::forward<T>(x);
      return *this;
    }
  };
} // namespace rs

// Fails the test if the given condition is true. (testassert macro also available)
// Can append info about failure using '<<', the insertion operator.
#define failif(cond) \
if (cond) \
  failtest << "failif(" << #cond << "); "

// Fails the test, saving the current filename and line number. 
// Can append info about failure using '<<', the insertion operator.
#define failtest throw rs::test_failure_exception(__FILE__, __LINE__)

// Fails the test if the given condition is NOT true. (failif macro also available)
// Can append info about failure using '<<', the insertion operator.
#define testassert(cond) \
if (!(cond)) \
  failtest << "testassert(" << #cond << "); "
