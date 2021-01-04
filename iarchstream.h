#pragma once
#ifndef IARCHSTREAM_H
#define IARCHSTREAM_H

#include <iostream>
#include <fstream>

#include <vector>
#include <string>
#include <list>

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
    size_t read_size(const T& container)
    {
      size_t size;
      stream.read((char*)&size, sizeof(size));
      return size;
    }

    // vector
    template <class T>
    void operator>>(std::vector<T>& container)
    {
      size_t size = read_size(container);
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
      size_t size = read_size(container);

      T element;
      for (size_t i = 0; i < size; ++i) {
        stream.read((char*)&element, sizeof(element));
        container.emplace_back(std::move(element));
      }
    }

    // string
    void operator>>(std::string& container)
    {
      size_t size = read_size(container);
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