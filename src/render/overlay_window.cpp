// src/render/overlay_window.cpp
#include "makadi/render/overlay_window.hpp"

#include "makadi/behaviors/flee_from_pointer.hpp"
#include "makadi/behaviors/flee_from_pointer_and_turn_away.hpp"

#include "makadi/platform/overlay_platform.hpp"

#include <QApplication>
#include <QCursor>
#include <QGuiApplication>
#include <QPainter>
#include <QScreen>
#include <QShortcut>
#include <QKeySequence>

#include <iostream>
#include <cmath>

class OverlayPointerProvider final : public makadi::input::PointerProvider {
public:
  explicit OverlayPointerProvider(const QWidget& window)
  : window_(window)
  {}

  QPointF pointerPosition() const override
  {
    const QPointF global = makadi::platform::globalPointerPosition();
    return window_.mapFromGlobal(global.toPoint());
  }

private:
  const QWidget& window_;
};

namespace makadi::render {

OverlayWindow::OverlayWindow(
  bool debug,
  const makadi::config::AppConfig& config,
  QWidget* parent)
: QWidget(parent),
  pointer_provider_(*this),
  debug_(debug),
  config_(config)
{
  setWindowFlags(
    Qt::FramelessWindowHint |
    Qt::WindowStaysOnTopHint |
    Qt::Tool |
    Qt::WindowDoesNotAcceptFocus);

  setAttribute(Qt::WA_TranslucentBackground);
  setAttribute(Qt::WA_NoSystemBackground);
  setAttribute(Qt::WA_TransparentForMouseEvents);

  const QRect screen_rect = QGuiApplication::primaryScreen()->availableGeometry();
  setGeometry(screen_rect);

  world_.mainEntity().radius = config_.asset.radius;
  world_.mainEntity().pose.x = width() / 2.0;
  world_.mainEntity().pose.y = height() / 2.0;
  world_.mainEntity().pose.theta = makadi::core::Angle::fromRadians(0.0);

  if (config_.asset.type == makadi::config::AssetType::Image) {
    has_entity_image_ = entity_image_.load(config_.asset.path);

    if (!has_entity_image_) {
      std::cerr << "[makadi] Failed to load image: "
                << config_.asset.path.toStdString() << "\n";
    }
  }

  if (!config_.animation.walking.frames_folder.isEmpty()) {
    walking_animation_.setSpeedToFps(config_.animation.walking.speed_to_fps);
    walking_animation_.setMinFps(config_.animation.walking.min_fps);
    walking_animation_.setMaxFps(config_.animation.walking.max_fps);

    has_walking_animation_ =
      walking_animation_.loadFromFolder(config_.animation.walking.frames_folder);

    if (debug_) {
      std::cerr
        << "[makadi debug] Walking animation folder: "
        << config_.animation.walking.frames_folder.toStdString() << "\n"
        << "[makadi debug] Walking animation loaded: "
        << (has_walking_animation_ ? "true" : "false") << "\n"
        << "[makadi debug] Walking frame count: "
        << walking_animation_.frameCount() << "\n";
    }
  }

  if (debug_) {
    std::cerr << "[makadi debug] Asset type: ";

    // Asset logging
    if (config_.asset.type == makadi::config::AssetType::Image) {
      std::cerr << "image\n";
      std::cerr << "[makadi debug] Asset path: "
                << config_.asset.path.toStdString() << "\n";
    } else {
      std::cerr << "circle\n";
      std::cerr << "[makadi debug] Asset color: "
                << config_.asset.color.toStdString() << "\n";
    }

    std::cerr << "[makadi debug] Asset radius: "
              << config_.asset.radius << "\n";

    std::cerr << "[makadi debug] Behavior type: ";

    // Behavior logging
    switch (config_.behavior.type) {
      case makadi::config::BehaviorType::FleeFromPointer:
        std::cerr << "flee_from_pointer\n";
        break;
      case makadi::config::BehaviorType::FleeFromPointerAndTurnAway:
        std::cerr << "flee_from_pointer_and_turn_away\n";
        break;
    }

    std::cerr
      << "[makadi debug] Behavior flee_radius: " << config_.behavior.flee_radius << "\n"
      << "[makadi debug] Behavior max_speed: " << config_.behavior.max_speed << "\n"
      << "[makadi debug] Behavior damping: " << config_.behavior.damping << "\n"
      << "[makadi debug] Behavior turn_gain: " << config_.behavior.turn_gain << "\n";

    // Animation logging
    std::cerr
      << "[makadi debug] Behavior min_speed: " << config_.behavior.min_speed << "\n"
      << "[makadi debug] Walking frames_folder: "
      << config_.animation.walking.frames_folder.toStdString() << "\n"
      << "[makadi debug] Walking speed_to_fps: "
      << config_.animation.walking.speed_to_fps << "\n"
      << "[makadi debug] Walking fps range: "
      << config_.animation.walking.min_fps << " - "
      << config_.animation.walking.max_fps << "\n";
  }

  if (config_.asset.type == makadi::config::AssetType::Image) {
    has_entity_image_ = entity_image_.load(config_.asset.path);

    if (debug_) {
      if (has_entity_image_) {
        std::cerr << "[makadi debug] Loaded image asset successfully\n";
      } else {
        std::cerr << "[makadi debug] Failed to load image asset\n";
      }
    }
  }

  auto* quit_shortcut = new QShortcut(QKeySequence("Ctrl+Q"), this);
  connect(quit_shortcut, &QShortcut::activated, qApp, &QApplication::quit);

  std::unique_ptr<core::BehaviorNode> behavior;

  switch (config_.behavior.type) {
    case makadi::config::BehaviorType::FleeFromPointer: {
      auto node = std::make_unique<behaviors::FleeFromPointer>(
        world_.mainEntity(),
        pointer_provider_);

      node->setFleeRadius(config_.behavior.flee_radius);
      node->setMaxSpeed(config_.behavior.max_speed);
      node->setDamping(config_.behavior.damping);

      behavior = std::move(node);
      break;
    }

    case makadi::config::BehaviorType::FleeFromPointerAndTurnAway: {
      auto node = std::make_unique<behaviors::FleeFromPointerAndTurnAway>(
        world_.mainEntity(),
        pointer_provider_);

      node->setFleeRadius(config_.behavior.flee_radius);
      node->setMinSpeed(config_.behavior.min_speed);
      node->setMaxSpeed(config_.behavior.max_speed);
      node->setDamping(config_.behavior.damping);
      node->setTurnGain(config_.behavior.turn_gain);

      behavior = std::move(node);
      break;
    }
  }

  world_.setBehavior(std::move(behavior));

  clock_.start();

  connect(&timer_, &QTimer::timeout, this, &OverlayWindow::tick);
  timer_.start(16);

  makadi::platform::makeInputPassthrough(*this);
}

void OverlayWindow::tick()
{
  const double dt = clock_.restart() / 1000.0;
  last_dt_sec_ = dt;
  
  world_.update(dt);

  auto& entity = world_.mainEntity();

  entity.pose.x = std::clamp(entity.pose.x, 0.0, double(width()));
  entity.pose.y = std::clamp(entity.pose.y, 0.0, double(height()));

  if (debug_) {
    debug_accum_sec_ += dt;

    if (debug_accum_sec_ >= 0.25) {
      debug_accum_sec_ = 0.0;

      const QPointF global_pointer =
        makadi::platform::globalPointerPosition();

      const QPointF local_pointer =
        pointer_provider_.pointerPosition();

      std::cerr
        << "[makadi debug] "
        << "window_pos=(" << x() << ", " << y() << ") "
        << "window_size=(" << width() << ", " << height() << ") "
        << "global_pointer=(" << global_pointer.x() << ", " << global_pointer.y() << ") "
        << "local_pointer=(" << local_pointer.x() << ", " << local_pointer.y() << ") "
        << "entity=(" << entity.pose.x << ", " << entity.pose.y
        << ", " << entity.pose.theta.radians() << ") "
        << "velocity=(" << entity.velocity.x() << ", " << entity.velocity.y() << ")"
        << std::endl;
    }
  }

  update();
}

void OverlayWindow::paintEvent(QPaintEvent*)
{
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  const auto& entity = world_.mainEntity();

  painter.save();

  painter.translate(entity.pose.x, entity.pose.y);
  painter.rotate(entity.pose.theta.degrees());

  const double size = entity.radius * 2.0;

  QRectF target(
    -entity.radius,
    -entity.radius,
    size,
    size);

  const double speed =
    std::hypot(entity.velocity.x(), entity.velocity.y());

  if (has_walking_animation_) {
    const QPixmap& frame =
      speed > 1e-3
        ? walking_animation_.frameAt(speed, last_dt_sec_)
        : walking_animation_.currentFrame();

    painter.drawPixmap(target, frame, frame.rect());
  } else if (config_.asset.type == makadi::config::AssetType::Image && has_entity_image_) {
    painter.drawPixmap(target, entity_image_, entity_image_.rect());
  } else {
    painter.setBrush(QColor(config_.asset.color));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(QPointF(0.0, 0.0), entity.radius, entity.radius);
  }

  painter.restore();
}

}  // namespace makadi::render