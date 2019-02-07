#pragma once

#include "fwd.h"

namespace schema {

struct IComponentContainer {
  virtual void freeBlock(ComponentId) = 0;
};

} // namespace schema
