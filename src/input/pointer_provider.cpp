#include "makadi/input/pointer_provider.hpp"

#include "makadi/platform/overlay_platform.hpp"

namespace makadi::input {

OverlayPointerProvider::OverlayPointerProvider(const QWidget& window)
: window_(window)
{}

QPointF OverlayPointerProvider::pointerPosition() const
{
  const QPointF global = makadi::platform::globalPointerPosition();
  return window_.mapFromGlobal(global.toPoint());
}

}  // namespace makadi::input