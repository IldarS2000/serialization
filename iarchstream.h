#pragma once
#ifndef IARCHSTREAM_H
#define IARCHSTREAM_H

#include <iostream>
#include <fstream>

#include "containers.h"

#include <algorithm>
#include <iterator>
#include <type_traits>


namespace archive
{
  class iarchstream
  {
  private:
    std::ifstream& stream;

  public:
    iarchstream(std::ifstream& stream) : stream(stream) {}

    template <class T>
    size_t readSize(const T& container)
    {
      size_t size;
      stream.read((char*)&size, sizeof(size));
      return size;
    }

    // fundamental or user type
    template <class T>
    void operator>>(T& object)
    {
      stream.read((char*)&object, sizeof(object));
    }

    // vector
    template <class T>
    void operator>>(std::vector<T>& container)
    {
      size_t size = readSize(container);
      container.reserve(size);

      T element;
      for (size_t i = 0; i < size; ++i) {
        stream.read((char*)&element, sizeof(element));
        container.emplace_back(std::move(element));
      }
    }

    // list
    template <class T>
    void operator>>(std::list<T>& container)
    {
      size_t size = readSize(container);

      T element;
      for (size_t i = 0; i < size; ++i) {
        stream.read((char*)&element, sizeof(element));
        container.emplace_back(std::move(element));
      }
    }

    // deque
    template <class T>
    void operator>>(std::deque<T>& container)
    {
      size_t size = readSize(container);

      T element;
      for (size_t i = 0; i < size; ++i) {
        stream.read((char*)&element, sizeof(element));
        container.emplace_back(std::move(element));
      }
    }

    // forward_list
    template <class T>
    void operator>>(std::forward_list<T>& container)
    {
      std::vector<T> temp;
      *this >> temp;
      std::copy(temp.rbegin(), temp.rend(), std::front_inserter(container));
    }

    // string
    void operator>>(std::string& container)
    {
      size_t size = readSize(container);
      container.reserve(size);

      char element;
      for (size_t i = 0; i < size; ++i) {
        stream.read((char*)&element, sizeof(element));
        container.push_back(std::move(element));
      }
    }
  };
}


#endif // IARCHSTREAM_H