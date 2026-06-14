// src/behaviors/flee_from_pointer_and_turn_away.cpp
#include "makadi/behaviors/flee_from_pointer_and_turn_away.hpp"

#include <cmath>
#include <algorithm>

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
  const bool pointer_in_radius = dist > 1.0 && dist < flee_radius_;

  if (pointer_in_radius) {
    const double strength = 1.0 - dist / flee_radius_;
    const QPointF dir = away / dist;

    entity_.velocity += dir * max_speed_ * strength * dt_sec;

    const double speed =
      std::hypot(entity_.velocity.x(), entity_.velocity.y());

    if (speed > 1e-6 && speed < min_speed_) {
      entity_.velocity *= min_speed_ / speed;
    }

    const core::Angle desired_heading =
      core::Angle::fromRadians(std::atan2(dir.y(), dir.x())) + heading_offset_;

    const core::Angle heading_error =
      desired_heading - entity_.pose.theta;

    double angular_step =
      heading_error.radians() * turn_gain_ * dt_sec;

    const double max_step = max_turn_speed_ * dt_sec;

    angular_step = std::clamp(angular_step, -max_step, max_step);

    entity_.pose.theta += core::Angle::fromRadians(angular_step);
  } else {
    entity_.velocity = QPointF(0.0, 0.0);
  }

  entity_.velocity *= damping_;

  entity_.pose.x += entity_.velocity.x() * dt_sec;
  entity_.pose.y += entity_.velocity.y() * dt_sec;

  return core::BehaviorStatus::Running;
}

}  // namespace makadi::behaviors