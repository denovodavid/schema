#pragma once

#include "fwd.h"
#include "i_component.h"

namespace schema {

template <class T> class Component : public IComponent {
public:
  static const ComponentTypeId ComponentTypeId;
};

template <class T>
const ComponentTypeId
    Component<T>::ComponentTypeId = TypeCounter<IComponent>::get<T>();

} // namespace schema
