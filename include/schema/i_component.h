#pragma once

#include "fwd.h"
#include "util/type_counter.h"

namespace schema {

struct IComponent {};

template <> ComponentTypeId TypeCounter<IComponent>::Count{0};

template <class T> static constexpr bool isComponent() {
  // check if T is IComponent at compile time.
  return std::is_base_of<IComponent, T>::value;
}

} // namespace schema
