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

template<class T>
void assertEqual(const T& actual, const T& expected, const char* filename, unsigned line, const char* typeName)
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
    std::cout << typeName << ": ok\n";
  }
}
#define ASSERT_EQUAL(actual, expected, type) assertEqual(actual, expected, __FILE__, __LINE__, type)

template<class T>
void testObject(const T& value, const char* typeName)
{
  {
    T expected = value;
    {
      std::ofstream out("data.bin", std::ios::binary);
      archive::oarchstream stream(out);
      stream << expected;
    }

    T actual;
    {
      std::ifstream in("data.bin", std::ios::binary);
      archive::iarchstream stream(in);
      stream >> actual;
    }

    ASSERT_EQUAL(actual, expected, typeName);
  }
}

void tests()
{
  testObject(69420, "int");
  testObject(3.14, "double");
  testObject('x', "char");
  testObject(true, "bool");

  testObject(std::vector<int>{ 1, 2, 3, 4, 5 }, "vector<int>");
  testObject(std::list<int>{ 1, 2, 3, 4, 5 }, "list<int>");
  testObject(std::string{ "hello world!" }, "string");

  std::cout << "-------------\n";
  std::cout << "all tests ok!\n";
}


int main()
{
  try {
    tests();
  }
  catch (const std::exception& e) {
    system("cls");
    std::cout << e.what() << '\n';
  }

  return 0;
}
