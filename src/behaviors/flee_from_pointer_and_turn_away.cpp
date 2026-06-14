// src/behaviors/flee_from_pointer_and_turn_away.cpp
#include "makadi/behaviors/flee_from_pointer_and_turn_away.hpp"

#include <cmath>

namespace makadi::behaviors {

FleeFromPointerAndTurnAway::FleeFromPointerAndTurnAway(
  core::Entity& entity,
  const input::PointerProvider& pointer_provider)
: entity_(entity),
  pointer_provider_(pointer_provider)
{}

core::BehaviorStatus FleeFromPointerAndTurnAway::tick(double dt_sec)
{
  const QPointF pointer = pointer_provider_.pointerPosition();
  const QPointF position(entity_.pose.x, entity_.pose.y);
  const QPointF away = position - pointer;

  const double dist = std::hypot(away.x(), away.y());

  if (dist > 1.0 && dist < flee_radius_) {
    const double strength = 1.0 - dist / flee_radius_;
    const QPointF dir = away / dist;

    entity_.velocity += dir * max_speed_ * strength * dt_sec;

    const core::Angle desired_heading =
      core::Angle::fromRadians(std::atan2(dir.y(), dir.x()));

    const core::Angle heading_error =
      desired_heading - entity_.pose.theta;

    entity_.pose.theta += heading_error * turn_gain_ * dt_sec;
  }

  entity_.velocity *= damping_;
  entity_.pose.x += entity_.velocity.x() * dt_sec;
  entity_.pose.y += entity_.velocity.y() * dt_sec;

  return core::BehaviorStatus::Running;
}

}  // namespace makadi::behaviors