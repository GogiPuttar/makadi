// include/makadi/core/entity.hpp
#pragma once

#include <QPointF>

namespace makadi::core {

struct Entity {
  QPointF position;
  QPointF velocity;
  double radius = 24.0;
};

}  // namespace makadi::core