// include/makadi/behaviors/flee_from_pointer_and_turn_away.hpp
#pragma once

#include "makadi/core/behavior_tree.hpp"
#include "makadi/core/entity.hpp"
#include "makadi/input/pointer_provider.hpp"

namespace makadi::behaviors {

class FleeFromPointerAndTurnAway final : public core::BehaviorNode {
public:
  FleeFromPointerAndTurnAway(
    core::Entity& entity,
    const input::PointerProvider& pointer_provider);

  core::BehaviorStatus tick(double dt_sec) override;

  void setFleeRadius(double value) { flee_radius_ = value; }
  void setMaxSpeed(double value) { max_speed_ = value; }
  void setDamping(double value) { damping_ = value; }
  void setTurnGain(double value) { turn_gain_ = value; }

private:
  core::Entity& entity_;
  const input::PointerProvider& pointer_provider_;

  double flee_radius_ = 160.0;
  double max_speed_ = 500.0;
  double damping_ = 0.90;
  double turn_gain_ = 12.0;
};

}  // namespace makadi::behaviors