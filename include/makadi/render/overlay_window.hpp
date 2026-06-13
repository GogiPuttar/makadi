// include/makadi/render/overlay_window.hpp
#pragma once

#include "makadi/core/world.hpp"
#include "makadi/input/pointer_provider.hpp"

#include <QElapsedTimer>
#include <QTimer>
#include <QWidget>

#include <memory>

namespace makadi::render {

class OverlayWindow final : public QWidget {
public:
  explicit OverlayWindow(QWidget* parent = nullptr);

protected:
  void paintEvent(QPaintEvent* event) override;

private:
  core::World world_;
  input::OverlayPointerProvider pointer_provider_;

  QTimer timer_;
  QElapsedTimer clock_;

  void tick();
};

}  // namespace makadi::render