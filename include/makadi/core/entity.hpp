#pragma once

#include "makadi/core/pose2d.hpp"

#include <QPointF>

namespace makadi::core {

struct Entity {
  Pose2D pose;
  QPointF velocity;
  double angular_velocity = 0.0;
  double radius = 24.0;
};

}  // namespace makadi::core