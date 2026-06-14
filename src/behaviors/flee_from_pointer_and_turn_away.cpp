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
  const QPointF raw_pointer = pointer_provider_.pointerPosition();

  if (!has_filtered_pointer_) {
    filtered_pointer_ = raw_pointer;
    has_filtered_pointer_ = true;
  } else {
    filtered_pointer_ =
      raw_pointer * pointer_filter_alpha_ +
      filtered_pointer_ * (1.0 - pointer_filter_alpha_);
  }

  const QPointF pointer = filtered_pointer_;
  const QPointF position(entity_.pose.x, entity_.pose.y);
  const QPointF away = position - pointer;

  const double dist = std::hypot(away.x(), away.y());

  if (!fleeing_ && dist < flee_radius_) {
    fleeing_ = true;
  }

  if (fleeing_ && dist > safe_radius_) {
    fleeing_ = false;
  }

  const bool should_flee = fleeing_ && dist > 1.0;

  if (should_flee) {
    const double strength = 1.0 - dist / flee_radius_;
    const QPointF dir = away / dist;

    const double target_speed =
      std::max(min_speed_, max_speed_ * strength);

    const QPointF desired_velocity =
      dir * target_speed;

    const double velocity_tracking_gain = 12.0;

    entity_.velocity +=
      (desired_velocity - entity_.velocity) *
      std::clamp(velocity_tracking_gain * dt_sec, 0.0, 1.0);

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