// include/makadi/render/overlay_window.hpp
#pragma once

#include "makadi/core/world.hpp"
#include "makadi/input/pointer_provider.hpp"
#include "makadi/config/app_config.hpp"
#include "makadi/render/animated_sprite.hpp"

#include <QElapsedTimer>
#include <QTimer>
#include <QWidget>

#include <memory>

namespace makadi::render {

class OverlayWindow final : public QWidget {
public:
  explicit OverlayWindow(
    bool debug,
    const makadi::config::AppConfig& config,
    QWidget* parent = nullptr);

protected:
  void paintEvent(QPaintEvent* event) override;

private:
  core::World world_;
  input::OverlayPointerProvider pointer_provider_;

  QTimer timer_;
  QElapsedTimer clock_;

  bool debug_ = false;
  double debug_accum_sec_ = 0.0;

  makadi::config::AppConfig config_;
  QPixmap entity_image_;
  bool has_entity_image_ = false;

  AnimatedSprite walking_animation_;
  bool has_walking_animation_ = false;
  double last_dt_sec_ = 0.0;

  void tick();
};

}  // namespace makadi::render