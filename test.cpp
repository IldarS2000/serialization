#include <iostream>
#include <fstream>

#include <vector>
#include <string>
#include <list>
#include <algorithm>

#include <cassert>

#include "oarchstream.h"
#include "iarchstream.h"


template<class T>
std::ostream& operator<<(std::ostream& out, std::vector<T> vec)
{
  std::copy(vec.begin(), vec.end(), std::ostream_iterator<T>(std::cout, " "));
  return out;
}

template<class T>
std::ostream& operator<<(std::ostream& out, std::list<T> vec)
{
  std::copy(vec.begin(), vec.end(), std::ostream_iterator<T>(std::cout, " "));
  return out;
}


void tests()
{
  {
    std::vector<int> expectation = { 1, 2, 3, 4, 5 };

    {
      std::ofstream out("data.bin", std::ios::binary);
      archive::oarchstream stream(out);
      stream << expectation;
    }

    std::vector<int> reality;
    {
      std::ifstream in("data.bin", std::ios::binary);
      archive::iarchstream stream(in);
      stream >> reality;
    }

    assert(expectation == reality);
    std::cout << "vector<int> ok\n";
  }

  {
    std::list<int> expectation = { 1, 2, 3, 4, 5 };

    {
      std::ofstream out("data.bin", std::ios::binary);
      archive::oarchstream stream(out);
      stream << expectation;
    }

    std::list<int> reality;
    {
      std::ifstream in("data.bin", std::ios::binary);
      archive::iarchstream stream(in);
      stream >> reality;
    }

    assert(expectation == reality);
    std::cout << "list<int> ok\n";
  }

  {
    std::string expectation = "hello world!";

    {
      std::ofstream out("data.bin", std::ios::binary);
      archive::oarchstream stream(out);
      stream << expectation;
    }

    std::string reality;
    {
      std::ifstream in("data.bin", std::ios::binary);
      archive::iarchstream stream(in);
      stream >> reality;
    }

    assert(expectation == reality);
    std::cout << "string ok\n";
  }


  std::cout << "-------------\n";
  std::cout << "all tests ok!\n";
}


int main()
{
  tests();

  return 0;
}
