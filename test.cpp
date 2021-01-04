#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <string>
#include <list>
#include <algorithm>

#include <cassert>
#include <exception>

#include "oarchstream.h"
#include "iarchstream.h"


template<class T>
std::ostream& operator<<(std::ostream& out, std::vector<T> vec)
{
  std::copy(vec.begin(), vec.end(), std::ostream_iterator<T>(out));
  return out;
}

template<class T>
std::ostream& operator<<(std::ostream& out, std::list<T> vec)
{
  std::copy(vec.begin(), vec.end(), std::ostream_iterator<T>(out));
  return out;
}

template<typename T>
void assertEqual(const T& actual, const T& expected, const char* filename, unsigned line, const char* type)
{
  if (actual != expected) {
    std::ostringstream message;
    message << "Not equal error occured.\n";
    message << "Expected: <" << expected << ">\n";
    message << "Actual:   <" << actual << ">\n";
    message << "file: " << filename << '\n';
    message << "line: " << line << '\n';
    std::string str = message.str();

    throw std::exception(str.c_str());
  }
  else {
    std::cout << type << ": ok\n";
  }
}
#define ASSERT_EQUAL(actual, expected, type) assertEqual(actual, expected, __FILE__, __LINE__, type)

void tests()
{
  {
    std::vector<int> expected = { 1, 2, 3, 4, 5 };
    {
      std::ofstream out("data.bin", std::ios::binary);
      archive::oarchstream stream(out);
      stream << expected;
    }

    std::vector<int> actual;
    {
      std::ifstream in("data.bin", std::ios::binary);
      archive::iarchstream stream(in);
      stream >> actual;
    }

    ASSERT_EQUAL(actual, expected, "vector<int>");
  }

  {
    std::list<int> expected = { 1, 2, 3, 4, 5 };
    {
      std::ofstream out("data.bin", std::ios::binary);
      archive::oarchstream stream(out);
      stream << expected;
    }

    std::list<int> actual;
    {
      std::ifstream in("data.bin", std::ios::binary);
      archive::iarchstream stream(in);
      stream >> actual;
    }

    ASSERT_EQUAL(actual, expected, "list<int>");
  }

  {
    std::string expected = "hello world!";
    {
      std::ofstream out("data.bin", std::ios::binary);
      archive::oarchstream stream(out);
      stream << expected;
    }

    std::string actual;
    {
      std::ifstream in("data.bin", std::ios::binary);
      archive::iarchstream stream(in);
      stream >> actual;
    }

    ASSERT_EQUAL(actual, expected, "string");
  }

  std::cout << "-------------\n";
  std::cout << "all tests ok!\n";
}


int main()
{
  try {
    tests();
  }
  catch (std::exception& e) {
    system("cls");
    std::cout << e.what() << '\n';
  }

  return 0;
}
