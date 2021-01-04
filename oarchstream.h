#pragma once
#ifndef OARCHSTREAM_H
#define OARCHSTREAM_H


#include <fstream>

#include <vector>
#include <string>
#include <list>

#include <algorithm>
#include <iterator>
#include <type_traits>


namespace archive
{
  class oarchstream
  {
  private:
    std::ofstream& stream;

  public:
    oarchstream(std::ofstream& stream) : stream(stream) {}

    template <class T>
    void write_size(const T& container)
    {
      size_t size = container.size();
      stream.write((char*)&size, sizeof(size));
    }

    template <class T>
    void write(const T& val)
    {
      if constexpr (std::is_fundamental<T>::value) {
        stream.write((char*)&val, sizeof(val));
      }
      else {
        val.serialize(*this);
      }
    }

    // vector
    template <class T>
    void operator<<(const std::vector<T>& container)
    {
      write_size(container);
      for (const auto& element : container) {
        write(element);
      }
    }

    // list
    template <class T>
    void operator<<(const std::list<T>& container)
    {
      write_size(container);
      for (const auto& element : container) {
        write(element);
      }
    }

    // string
    void operator<<(const std::string& container)
    {
      write_size(container);
      for (const auto& element : container) {
        write(element);
      }
    }
  };
}


#endif // OARCHSTREAM_H