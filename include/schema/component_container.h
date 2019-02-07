#pragma once

#include "fwd.h"
#include "i_component_container.h"

namespace schema {

template <class T, const size_t MaxComponents>
class ComponentContainer : public IComponentContainer {
private:
  std::list<ComponentId> FreeBlocks;
  T *Blocks = new T[MaxComponents];

public:
  ComponentContainer() {
    for (int i = 0; i < MaxComponents; i++)
      FreeBlocks.push_back(i);
  }

  ~ComponentContainer() { delete[] Blocks; }

  template <class... ARGS> ComponentId insert(ARGS &&... args) {
    assert(FreeBlocks.size() > 0 && "Not enough memory!");
    ComponentId cId = FreeBlocks.back();
    FreeBlocks.pop_back();
    Blocks[cId] = T(std::forward<ARGS>(args)...);
    return cId;
  }

  T &at(ComponentId cId) { return Blocks[cId]; }

  T *getBlock() {
    if (FreeBlocks.size() == 0)
      return nullptr;
    T *ptr = &(Blocks[FreeBlocks.back()]);
    FreeBlocks.pop_back();
    return ptr;
  }

  void freeBlock(ComponentId cId) override { FreeBlocks.push_back(cId); }
};

} // namespace schema
