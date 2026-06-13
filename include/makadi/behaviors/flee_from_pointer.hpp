// include/makadi/behaviors/flee_from_pointer.hpp
#pragma once

#include "makadi/core/behavior_tree.hpp"
#include "makadi/core/entity.hpp"
#include "makadi/input/pointer_provider.hpp"

namespace makadi::behaviors {

class FleeFromPointer final : public core::BehaviorNode {
public:
  FleeFromPointer(
    core::Entity& entity,
    const input::PointerProvider& pointer_provider);

  core::BehaviorStatus tick(double dt_sec) override;

private:
  core::Entity& entity_;
  const input::PointerProvider& pointer_provider_;

  double flee_radius_ = 160.0;
  double max_speed_ = 500.0;
  double damping_ = 0.90;
};

}  // namespace makadi::behaviors