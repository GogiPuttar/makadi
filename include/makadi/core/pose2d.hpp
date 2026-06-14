#pragma once

#include "makadi/core/angle.hpp"

namespace makadi::core {

struct Pose2D {
  double x = 0.0;
  double y = 0.0;
  Angle theta = Angle::fromRadians(0.0);
};

}  // namespace makadi::core