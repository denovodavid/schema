#pragma once

#include "fwd.h"

namespace schema {

template <class COMPONENT_TYPE_BITSET, class ECS> struct ISystem {
  // should be a static const thing can check at compile time or whatever...
  virtual const COMPONENT_TYPE_BITSET getComponentTypeBitset() const = 0;
  virtual void onUpdate(const std::vector<EntityId> &, const ECS &) = 0;
};

} // namespace schema
