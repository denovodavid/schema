#include "schema.h"
#include <iostream>

constexpr size_t MAX_COMPONENT_TYPES = 64;
constexpr size_t MAX_COMPONENTS = 64;

using ECS = schema::ECS<MAX_COMPONENT_TYPES, MAX_COMPONENTS>;

struct PositionComponent : public schema::Component<PositionComponent> {
  int32_t x, y;
  PositionComponent(){};
  PositionComponent(int32_t x, int32_t y) : x(x), y(y){};
};

struct VelocityComponent : public schema::Component<VelocityComponent> {
  int32_t x, y;
  VelocityComponent(){};
  VelocityComponent(int32_t x, int32_t y) : x(x), y(y){};
};

class PhysicsSystem : public ECS::ISystem {
private:
  const ECS::ComponentTypeBitset bitset = ECS::getComponentTypeBitset(
      {PositionComponent::ComponentTypeId, VelocityComponent::ComponentTypeId});

public:
  const ECS::ComponentTypeBitset getComponentTypeBitset() const override {
    return bitset;
  }

  void onUpdate(const std::vector<schema::EntityId> &eIds,
                const ECS &ecs) override {
    auto &positions = *ecs.getComponentContainer<PositionComponent>();
    auto &velocities = *ecs.getComponentContainer<VelocityComponent>();
    for (const auto &eId : eIds) {
      // TODO: make into function
      schema::ComponentId posId =
          ecs.EntityComponentMap.at(eId).at(PositionComponent::ComponentTypeId);
      schema::ComponentId velId =
          ecs.EntityComponentMap.at(eId).at(VelocityComponent::ComponentTypeId);

      PositionComponent &pos = positions.at(posId);
      VelocityComponent &vel = velocities.at(velId);

      pos.x += vel.x;
      pos.y += vel.y;
    }
  }
};

class LogPositionSystem : public ECS::ISystem {
private:
  ECS::ComponentTypeBitset bitset =
      ECS::getComponentTypeBitset({PositionComponent::ComponentTypeId});

public:
  const ECS::ComponentTypeBitset getComponentTypeBitset() const override {
    return bitset;
  }

  void onUpdate(const std::vector<schema::EntityId> &eIds,
                const ECS &ecs) override {
    auto &positions = *ecs.getComponentContainer<PositionComponent>();
    for (const auto &eId : eIds) {
      schema::ComponentId posId =
          ecs.EntityComponentMap.at(eId).at(PositionComponent::ComponentTypeId);
      PositionComponent &pos = positions.at(posId);
      std::cout << "E" << eId << ": " << pos.x << ", " << pos.y << '\n';
    }
  }
};

int main() {
  auto ecs = ECS();

  ecs.registerComponent<PositionComponent>();
  ecs.registerComponent<VelocityComponent>();

  for (int i = 0; i < 32; i++) {
    schema::EntityId eId = ecs.createEntity();
    ecs.addComponent<PositionComponent>(eId, i, i);
    ecs.addComponent<VelocityComponent>(eId, i, i);
  }

  for (int i = 0; i < 32; i++) {
    schema::EntityId eId = ecs.createEntity();
    ecs.addComponent<PositionComponent>(eId, i, i);
  }

  ecs.registerSystem<PhysicsSystem>();
  ecs.registerSystem<LogPositionSystem>();

  std::cout << "\n==== FRAME 1 ====\n";
  ecs.updateSystems();

  //  ecs.removeEntity(32);
  ecs.removeComponent<VelocityComponent>(3);
  schema::EntityId eId = ecs.createEntity();
  //  ecs.addComponent<PositionComponent>(eId, 911, 37);
  ecs.addComponent<VelocityComponent>(eId, 0, 0);

  std::cout << "\n==== FRAME 2 ====\n";
  ecs.updateSystems();

  return 0;
}
