#pragma once

#include <assert.h>
#include <bitset>
#include <list>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

#include "component.h"
#include "component_container.h"
#include "fwd.h"
#include "i_system.h"

namespace schema {

template <const size_t MAX_COMPONENT_TYPES = 0, const size_t MAX_COMPONENTS = 0>
class ECS {
  static_assert(MAX_COMPONENT_TYPES != 0,
                "MAX_COMPONENT_TYPES cannot be 0 (default).");
  static_assert(MAX_COMPONENTS != 0, "MAX_COMPONENTS cannot be 0 (default).");

public:
  using ComponentTypeBitset = std::bitset<MAX_COMPONENT_TYPES>;
  template <class T>
  using ComponentContainer = ComponentContainer<T, MAX_COMPONENTS>;
  using ISystem =
      ISystem<ComponentTypeBitset, ECS<MAX_COMPONENT_TYPES, MAX_COMPONENTS>>;

  template <class T> static constexpr bool IsSystem() {
    // check if T is ISystem at compile time.
    return std::is_base_of<ISystem, T>::value;
  }

  // a map of maps, to map entity IDs to maps of component type IDs to
  // component IDs, which are the index of the component data in the
  // respective component container. phew!
  std::unordered_map<EntityId, std::unordered_map<ComponentTypeId, ComponentId>>
      EntityComponentMap;

private:
  std::unordered_map<ComponentTypeId, IComponentContainer *>
      ComponentContainerMap;
  // map entity IDs to a bitset of the component types that entity has.
  std::unordered_map<EntityId, ComponentTypeBitset> EntityComponentTypeMap;
  std::list<ISystem *> Systems;
  EntityId NextEntityId = 0;

  const std::vector<EntityId> *getEntities(ComponentTypeBitset bitset) {
    auto entities = new std::vector<EntityId>();
    entities->reserve(MAX_COMPONENTS);
    for (const auto &e : EntityComponentTypeMap) {
      if ((bitset & e.second) == bitset) {
        entities->push_back(e.first);
      }
    }
    entities->shrink_to_fit();
    return entities;
  }

public:
  // ensure values are known at compile time :)
  static constexpr size_t MaxComponentTypes = MAX_COMPONENT_TYPES;
  static constexpr size_t MaxComponents = MAX_COMPONENTS;

  static ComponentTypeBitset
  getComponentTypeBitset(std::initializer_list<ComponentTypeId> ctIds) {
    ComponentTypeBitset bitset;
    for (auto ctId : ctIds)
      bitset[ctId] = true;
    return bitset;
  }

  template <class T> ComponentContainer<T> *getComponentContainer() const {
    // get unique component ID
    ComponentTypeId ctId = T::ComponentTypeId;
    ComponentContainer<T> *componentContainer = nullptr;

    // check if componentContainer exists and return it
    auto it = ComponentContainerMap.find(ctId);
    if (it == ComponentContainerMap.end())
      assert("Component container not found! Is component registered?");
    else
      componentContainer = static_cast<ComponentContainer<T> *>(it->second);

    return componentContainer;
  }

  template <class T> void registerComponent() {
    static_assert(isComponent<T>(), "T is not a component!");
    // TODO: check if component already registered.
    // TODO: check total component count larger that MAX_COMPONENT_TYPES.
    const ComponentTypeId ctId = T::ComponentTypeId;
    ComponentContainerMap[ctId] = new ComponentContainer<T>();
  }

  EntityId createEntity() {
    EntityId eId = NextEntityId;
    NextEntityId++;
    EntityComponentTypeMap[eId] = ComponentTypeBitset();
    return eId;
  }

  void removeEntity(EntityId eId) {
    // has no problems if entity doesn't exist.
    ComponentTypeBitset bitset = EntityComponentTypeMap[eId];
    EntityComponentTypeMap.erase(eId);
    for (size_t ctId = 0; ctId < bitset.size(); ctId++) {
      if (bitset[ctId]) {
        ComponentId cId = EntityComponentMap[eId][ctId];
        ComponentContainerMap[ctId]->freeBlock(cId);
      }
    }
    EntityComponentMap.erase(eId);
  }

  template <class T, class... ARGS>
  void addComponent(EntityId eId, ARGS &&... args) {
    static_assert(isComponent<T>(), "T is not a component!");
    ComponentContainer<T> &container = *getComponentContainer<T>();

    ComponentId cId = container.insert(std::forward<ARGS>(args)...);
    ComponentTypeId ctId = T::ComponentTypeId;

    auto it = EntityComponentTypeMap.find(eId);
    if (it == EntityComponentTypeMap.end()) {
      assert("Entity not found!");
    } else {
      it->second[ctId] = 1;
    }

    EntityComponentMap[eId][ctId] = cId;
  }

  template <class T> void removeComponent(EntityId eId) {
    static_assert(isComponent<T>(), "T is not a component!");

    ComponentTypeId ctId = T::ComponentTypeId;

    auto it = EntityComponentTypeMap.find(eId);
    if (it == EntityComponentTypeMap.end()) {
      assert("Entity not found!");
    } else {
      if (it->second[ctId] == 0) {
        assert("Component not found!");
      }
      it->second[ctId] = 0;
    }

    ComponentId cId = EntityComponentMap[eId][ctId];
    EntityComponentMap[eId].erase(ctId);
    ComponentContainer<T> &container = *getComponentContainer<T>();
    container.freeBlock(cId);
  }

  template <class T> void registerSystem() {
    static_assert(IsSystem<T>(), "T is not a System!");
    Systems.push_back(new T());
  }

  void updateSystems() {
    for (auto s : Systems) {
      const auto entities = getEntities(s->getComponentTypeBitset());
      s->onUpdate(*entities, *this);
      delete entities;
    }
  }
};

} // namespace schema
