#pragma once

#include <utility>
#include <typeindex>
#include <unordered_map>
#include <list>
#include <vector>

#include <boost/dynamic_bitset.hpp>

#include "util/TypeCounter.h"

namespace schema {
    using EntityId = size_t;
    using ComponentId = size_t;
    using ComponentTypeId = size_t;
    using ComponentTypeBitset = boost::dynamic_bitset<>;

    template <class K, class V>
    using HashMap = std::unordered_map<K, V>;

    const size_t MAX_COMPONENTS = 64;

    EntityId nextEntityId = 0;

    //
    // COMPONENT
    //

    struct IComponent {};

    template <>
    size_t TypeCounter<IComponent>::count { 0 };

    template <class T>
    class Component : public IComponent
    {
    public:
        static const size_t COMPONENT_TYPE_ID;
    };

    template<class T>
    const size_t Component<T>::COMPONENT_TYPE_ID = TypeCounter<IComponent>::Get<T>();

    //
    // COMPONENT CONTAINER
    //

    struct IComponentContainer {
        virtual void freeBlock(ComponentId) = 0;
    };

    template <class T>
    class ComponentContainer : public IComponentContainer {
    private:
        std::list<ComponentId> freeBlocks;
        T* blocks = new T[MAX_COMPONENTS];

    public:
        ComponentContainer() {
            for (int i = 0; i < MAX_COMPONENTS; i++)
                freeBlocks.push_back(i);
        }

        ~ComponentContainer() {
            delete[] blocks;
        }

        template <class ...ARGS>
        ComponentId insert(ARGS&&... args) {
            assert(freeBlocks.size() > 0 && "Not enough memory!");
            ComponentId cId = freeBlocks.back();
            freeBlocks.pop_back();
            blocks[cId] = T(std::forward<ARGS>(args)...);
            return cId;
        }

        T& at(ComponentId cId) {
            return blocks[cId];
        }

        T* getBlock() {
            if (freeBlocks.size() == 0) return nullptr;
            T* ptr = &(blocks[freeBlocks.back()]);
            freeBlocks.pop_back();
            return ptr;
        }

        void freeBlock(ComponentId cId) override {
            freeBlocks.push_back(cId);
        }
    };

    template <class T>
    static constexpr bool IsComponent() {
        // check if T is IComponent at compile time.
        return std::is_base_of<IComponent, T>::value;
    }

    // this could be list or vector I guess maybe
    HashMap<ComponentTypeId, IComponentContainer*> componentContainerMap;

    ComponentTypeBitset getComponentTypeBitset() {
        return ComponentTypeBitset(TypeCounter<IComponent>::Get());
    }

    ComponentTypeBitset getComponentTypeBitset(std::initializer_list<ComponentTypeId> ctIds) {
        auto bitset = getComponentTypeBitset();
        for (auto id : ctIds) bitset[id] = 1;
        return bitset;
    }

    template <class T>
    ComponentContainer<T>* getComponentContainer() {
        // get unique component ID
        ComponentTypeId ctId = T::COMPONENT_TYPE_ID;
        ComponentContainer<T>* componentContainer = nullptr;

        // check if componentContainer exists and return it
        auto it = componentContainerMap.find(ctId);
        if (it == componentContainerMap.end())
            assert("Component container not found! Is component registered?");
        else
            componentContainer = static_cast<ComponentContainer<T>*>(it->second);

        return componentContainer;
    }

    // a map of maps, to map entity IDs to maps of component type IDs to
    // component IDs, which are the index of the component data in the
    // respective component container. phew!
    HashMap<EntityId, HashMap<ComponentTypeId, ComponentId>> entityComponentMap;
    // map entity IDs to a bitset of the component types that entity has.
    HashMap<EntityId, ComponentTypeBitset> entityComponentTypeMap;

    template <class T>
    void registerComponent() {
        static_assert(IsComponent<T>(), "T is not a component!");
        // TODO: check if component already registered.
        // uses the size of the map to determine incremental index
        ComponentTypeId ctId = T::COMPONENT_TYPE_ID;
        componentContainerMap[ctId] = new ComponentContainer<T>();
    }

    template <class T, class ...ARGS>
    void addComponent(EntityId eId, ARGS&&... args) {
        static_assert(IsComponent<T>(), "T is not a component!");
        ComponentContainer<T>& container = *getComponentContainer<T>();

        ComponentId cId = container.insert(std::forward<ARGS>(args)...);
        ComponentTypeId ctId = T::COMPONENT_TYPE_ID;

        auto it = entityComponentTypeMap.find(eId);
        if (it == entityComponentTypeMap.end()) {
            assert("Entity not found!");
        } else {
            it->second[ctId] = 1;
        }

        entityComponentMap[eId][ctId] = cId;
    }

    template <class T>
    void removeComponent(EntityId eId) {
        static_assert(IsComponent<T>(), "T is not a component!");

        ComponentTypeId ctId = T::COMPONENT_TYPE_ID;

        auto it = entityComponentTypeMap.find(eId);
        if (it == entityComponentTypeMap.end()) {
            assert("Entity not found!");
        } else {
            if (it->second[ctId] == 0) {
                assert("Component not found!");
            }
            it->second[ctId] = 0;
        }

        ComponentId cId = entityComponentMap[eId][ctId];
        entityComponentMap[eId].erase(ctId);
        ComponentContainer<T>& container = *getComponentContainer<T>();
        container.freeBlock(cId);
    }

    EntityId createEntity() {
        EntityId eId = nextEntityId;
        nextEntityId++;
        entityComponentTypeMap[eId] = getComponentTypeBitset();
        return eId;
    }

    void removeEntity(EntityId eId) {
        // has no problems if entity doesn't exist.
        ComponentTypeBitset bitset = entityComponentTypeMap[eId];
        entityComponentTypeMap.erase(eId);
        for (ComponentTypeBitset::size_type ctId = 0; ctId < bitset.size(); ctId++) {
            if (bitset[ctId] == 1) {
                ComponentId cId = entityComponentMap[eId][ctId];
                componentContainerMap[ctId]->freeBlock(cId);
            }
        }
        entityComponentMap.erase(eId);
    }

    const std::vector<EntityId>* getEntities(ComponentTypeBitset bitset) {
        auto entities = new std::vector<EntityId>();
        entities->reserve(MAX_COMPONENTS);
        for (const auto& e : entityComponentTypeMap) {
            if ((bitset & e.second) == bitset) {
                entities->push_back(e.first);
            }
        }
        entities->shrink_to_fit();
        return entities;
    }

    struct ISystem {
        // should be a static const thing can check at compile time or whatever...
        virtual ComponentTypeBitset getComponentTypeBitset() = 0;
        virtual void onUpdate(const std::vector<EntityId>&) = 0;
    };

    template <class T>
    static constexpr bool IsSystem() {
        // check if T is ISystem at compile time.
        return std::is_base_of<ISystem, T>::value;
    }

    std::list<ISystem*> systems;

    template <class T>
    void registerSystem() {
        static_assert(IsSystem<T>(), "T is not a System!");
        systems.push_back(new T());
    }

    void updateSystems() {
        for (auto s : systems) {
            const auto entities = getEntities(s->getComponentTypeBitset());
            s->onUpdate(*entities);
            delete entities;
        }
    }
}
