// src/behaviors/flee_from_pointer.cpp
#include "makadi/behaviors/flee_from_pointer.hpp"

#include <QtMath>

namespace makadi::behaviors {

FleeFromPointer::FleeFromPointer(
  core::Entity& entity,
  const input::PointerProvider& pointer_provider)
: entity_(entity),
  pointer_provider_(pointer_provider)
{}

core::BehaviorStatus FleeFromPointer::tick(double dt_sec)
{
  const QPointF pointer = pointer_provider_.pointerPosition();
  const QPointF away = entity_.position - pointer;

  const double dist = std::hypot(away.x(), away.y());

  if (dist > 1.0 && dist < flee_radius_) {
    const double strength = 1.0 - dist / flee_radius_;
    const QPointF dir = away / dist;

    entity_.velocity += dir * max_speed_ * strength * dt_sec;
  }

  entity_.velocity *= damping_;
  entity_.position += entity_.velocity * dt_sec;

  return core::BehaviorStatus::Running;
}

}  // namespace makadi::behaviors