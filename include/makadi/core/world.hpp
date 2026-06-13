// include/makadi/core/world.hpp
#pragma once

#include "makadi/core/entity.hpp"
#include "makadi/core/behavior_tree.hpp"

#include <memory>

namespace makadi::core {

class World {
public:
  Entity& mainEntity();
  const Entity& mainEntity() const;

  void setBehavior(std::unique_ptr<BehaviorNode> behavior);
  void update(double dt_sec);

private:
  Entity entity_;
  std::unique_ptr<BehaviorNode> behavior_;
};

}  // namespace makadi::core