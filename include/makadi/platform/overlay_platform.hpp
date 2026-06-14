// include/makadi/platform/overlay_platform.hpp
#pragma once

#include <QPointF>
#include <QWidget>

namespace makadi::platform {

void makeInputPassthrough(QWidget& window);
QPointF globalPointerPosition();

}  // namespace makadi::platform