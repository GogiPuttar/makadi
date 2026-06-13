// src/render/overlay_window.cpp
#include "makadi/render/overlay_window.hpp"

#include "makadi/behaviors/flee_from_pointer.hpp"

#include "makadi/platform/overlay_platform.hpp"

#include <QApplication>
#include <QCursor>
#include <QGuiApplication>
#include <QPainter>
#include <QScreen>
#include <QShortcut>
#include <QKeySequence>

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

OverlayWindow::OverlayWindow(QWidget* parent)
: QWidget(parent),
  pointer_provider_(*this)
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

  world_.mainEntity().position = QPointF(width() / 2.0, height() / 2.0);

  auto* quit_shortcut = new QShortcut(QKeySequence("Ctrl+Q"), this);
  connect(quit_shortcut, &QShortcut::activated, qApp, &QApplication::quit);

  world_.setBehavior(
    std::make_unique<behaviors::FleeFromPointer>(
      world_.mainEntity(),
      pointer_provider_));

  clock_.start();

  connect(&timer_, &QTimer::timeout, this, &OverlayWindow::tick);
  timer_.start(16);

  makadi::platform::makeInputPassthrough(*this);
}

void OverlayWindow::tick()
{
  const double dt = clock_.restart() / 1000.0;
  world_.update(dt);

  auto& entity = world_.mainEntity();

  entity.position.setX(std::clamp(entity.position.x(), 0.0, double(width())));
  entity.position.setY(std::clamp(entity.position.y(), 0.0, double(height())));

  update();
}

void OverlayWindow::paintEvent(QPaintEvent*)
{
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  const auto& entity = world_.mainEntity();

  painter.setBrush(Qt::black);
  painter.setPen(Qt::NoPen);

  painter.drawEllipse(
    entity.position,
    entity.radius,
    entity.radius);
}

}  // namespace makadi::render