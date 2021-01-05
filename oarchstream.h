#pragma once
#ifndef OARCHSTREAM_H
#define OARCHSTREAM_H


#include <fstream>

#include "containers.h"

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
    void writeSize(const T& container)
    {
      size_t size = container.size();
      stream.write((char*)&size, sizeof(size));
    }

#if _HAS_CXX17
    template <class T>
    void writeObject(const T& object)
    {
      if constexpr (std::is_fundamental<T>::value) {
        stream.write((char*)&object, sizeof(object));
      }
      else {
        object.serialize(*this);
      }
    }
#else
    template<typename T>
    typename std::enable_if<std::is_fundamental<T>::value>::type writeObject(const T& object)
    {
      stream.write((char*)&object, sizeof(object));
    }
    template<typename T>
    typename std::enable_if<!std::is_fundamental<T>::value>::type writeObject(const T& object)
    {
      object.serialize(*this);
    }
#endif

    template <class T>
    void writeContainer(const T& container)
    {
      writeSize(container);
      for (const auto& element : container) {
        writeObject(element);
      }
    }

    // fundamental or user type
    template <class T>
    void operator<<(const T& object)
    {
      writeObject(object);
    }

    // vector
    template <class T>
    void operator<<(const std::vector<T>& container)
    {
      writeContainer(container);
    }

    // list
    template <class T>
    void operator<<(const std::list<T>& container)
    {
      writeContainer(container);
    }

    // deque
    template <class T>
    void operator<<(const std::deque<T>& container)
    {
      writeContainer(container);
    }

    // forward_list
    template <class T>
    void operator<<(const std::forward_list<T>& container)
    {
      auto size = std::distance(container.begin(), container.end());
      stream.write((char*)&size, sizeof(size));
      for (const auto& element : container) {
        writeObject(element);
      }
    }

    // string
    template<class T>
    void operator<<(const std::basic_string<T>& container)
    {
      writeContainer(container);
    }
  };
}


#endif // OARCHSTREAM_H