// src/core/world.cpp
#include "makadi/core/world.hpp"

namespace makadi::core {

Entity& World::mainEntity() { return entity_; }
const Entity& World::mainEntity() const { return entity_; }

void World::setBehavior(std::unique_ptr<BehaviorNode> behavior)
{
  behavior_ = std::move(behavior);
}

void World::update(double dt_sec)
{
  if (behavior_) {
    behavior_->tick(dt_sec);
  }
}

}  // namespace makadi::core