#pragma once

#include <QPointF>
#include <QWidget>

namespace makadi::input {

class PointerProvider {
public:
  virtual ~PointerProvider() = default;
  virtual QPointF pointerPosition() const = 0;
};

class OverlayPointerProvider final : public PointerProvider {
public:
  explicit OverlayPointerProvider(const QWidget& window);

  QPointF pointerPosition() const override;

private:
  const QWidget& window_;
};

}  // namespace makadi::input