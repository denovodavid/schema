#pragma once

// #include "boost/optional.hpp"
// #include "boost/variant.hpp"
#include "fwd.h"
#include <functional>
#include <iostream>
#include <optional>
#include <variant>

// https://youtu.be/SofC6c9xQv4?t=1431
// https://gitpitch.com/azriel91/ecs_paradigm#/12
// Look at boost::variant & boost::expected & boost::optional for the container.

namespace schema {

template <class T, const size_t MaxComponents> class GenerationalArena {
public:
  struct Index {
    const size_t index;
    const size_t generation;
  };

private:
  struct Occupied {
    size_t generation;
    T data;
  };

  struct Free {
    std::optional<size_t> nextFree;
  };

  using Entry = std::variant<Free, Occupied>;

  Entry *items = new Entry[MaxComponents];
  std::optional<size_t> freeListHead = 0;
  size_t generation = 0;
  size_t itemCount = 0;

  Free &getFree(size_t index) const { return std::get<Free>(items[index]); }

  Occupied &getOccupied(size_t index) const {
    return std::get<Occupied>(items[index]);
  }

public:
  GenerationalArena() {
    //     std::cout << "T Size: " << sizeof(T) << "\n";
    //     std::cout << "Free Size: " << sizeof(Free) << "\n";
    //     std::cout << "Occupied Size: " << sizeof(Occupied) << "\n";
    //     std::cout << "Entry Size: " << sizeof(Entry) << "\n";
    //     std::cout << "Items Size: " << sizeof(Entry) * MaxComponents << "\n";
    clear();
  }

  ~GenerationalArena() { delete[] items; }

  template <class... ARGS> const Index insert(ARGS &&... args) {
    assert(freeListHead.has_value() && "Not enough memory!");
    size_t index = freeListHead.value();
    freeListHead = getFree(index).nextFree.value();
    items[index] = Occupied{generation, T(std::forward<ARGS>(args)...)};
    itemCount++;
    return Index{index, generation};
  }

  const std::optional<const std::reference_wrapper<T>> get(const Index &index) {
    const auto occupied = std::get_if<Occupied>(&items[index.index]);
    if (occupied && occupied->generation == index.generation) {
      return occupied->data;
    }
    return std::nullopt;
  }

  const std::optional<const std::reference_wrapper<const T>>
  get(const Index &index) const {
    const auto occupied = std::get_if<Occupied>(&items[index.index]);
    if (occupied && occupied->generation == index.generation) {
      return occupied->data;
    }
    return std::nullopt;
  }

  const std::optional<T> remove(const Index &index) {
    if (const auto item = get(index)) {
      items[index.index] = Free{freeListHead};
      freeListHead = index.index;
      generation++;
      itemCount--;
      return item;
    }
    return std::nullopt;
  }

  const bool contains(const Index &index) const {
    return get(index).has_value();
  }

  void clear() {
    items[MaxComponents - 1] = Free{std::nullopt};
    for (int i = MaxComponents - 2; i >= 0; --i) {
      items[i] = Free{i + 1};
    }
    freeListHead = 0;
    itemCount = 0;
  }

  const size_t size() const { return itemCount; }

  const bool empty() const { return itemCount == 0; }

  const size_t max_size() const { return MaxComponents; }

  T &operator[](const Index &index) { return get(index).value().get(); }

  const T &operator[](const Index &index) const {
    return get(index).value().get();
  }

  // TODO: make an iterator...
};

} // namespace schema
