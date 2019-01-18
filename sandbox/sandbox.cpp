#include <iostream>
#include "schema.h"

struct PositionComponent : public schema::Component<PositionComponent> {
  int x, y;
  PositionComponent(){};
  PositionComponent(int x, int y) : x(x), y(y){};
};

struct VelocityComponent : public schema::Component<VelocityComponent> {
  int x, y;
  VelocityComponent(){};
  VelocityComponent(int x, int y) : x(x), y(y){};
};

class PhysicsSystem : public schema::ISystem {
 private:
  schema::ComponentTypeBitset bitset =
      schema::getComponentTypeBitset({PositionComponent::COMPONENT_TYPE_ID,
                                      VelocityComponent::COMPONENT_TYPE_ID});

 public:
  schema::ComponentTypeBitset getComponentTypeBitset() override {
    return bitset;
  }

  void onUpdate(const std::vector<schema::EntityId>& eIds) override {
    auto& positions = *schema::getComponentContainer<PositionComponent>();
    auto& velocities = *schema::getComponentContainer<VelocityComponent>();
    for (const auto& eId : eIds) {
      schema::ComponentId posId =
          schema::entityComponentMap[eId][PositionComponent::COMPONENT_TYPE_ID];
      schema::ComponentId velId =
          schema::entityComponentMap[eId][VelocityComponent::COMPONENT_TYPE_ID];

      PositionComponent& pos = positions.at(posId);
      VelocityComponent& vel = velocities.at(velId);

      pos.x += vel.x;
      pos.y += vel.y;
    }
  }
};

class LogPositionSystem : public schema::ISystem {
 private:
  schema::ComponentTypeBitset bitset =
      schema::getComponentTypeBitset({PositionComponent::COMPONENT_TYPE_ID});

 public:
  schema::ComponentTypeBitset getComponentTypeBitset() override {
    return bitset;
  }

  void onUpdate(const std::vector<schema::EntityId>& eIds) override {
    auto& positions = *schema::getComponentContainer<PositionComponent>();
    for (const auto& eId : eIds) {
      schema::ComponentId posId =
          schema::entityComponentMap[eId][PositionComponent::COMPONENT_TYPE_ID];
      PositionComponent& pos = positions.at(posId);
      std::cout << "E" << eId << ": " << pos.x << ", " << pos.y << '\n';
    }
  }
};

int main() {
  // int x = 0b00000001;
  // std::cout << x << '\n';

  schema::registerComponent<PositionComponent>();
  schema::registerComponent<VelocityComponent>();

  for (int i = 0; i < 32; i++) {
    schema::EntityId eId = schema::createEntity();
    schema::addComponent<PositionComponent>(eId, i, i);
    schema::addComponent<VelocityComponent>(eId, i, i);
  }

  for (int i = 0; i < 32; i++) {
    schema::EntityId eId = schema::createEntity();
    schema::addComponent<PositionComponent>(eId, i, i);
  }

  schema::registerSystem<PhysicsSystem>();
  schema::registerSystem<LogPositionSystem>();

  std::cout << "\n==== FRAME 1 ====\n";
  schema::updateSystems();

  //    schema::removeEntity(32);
  schema::removeComponent<VelocityComponent>(3);
  schema::EntityId eId = schema::createEntity();
  //    schema::addComponent<PositionComponent>(eId, 911, 37);
  schema::addComponent<VelocityComponent>(eId, 0, 0);

  std::cout << "\n==== FRAME 2 ====\n";
  schema::updateSystems();

  return 0;
}
