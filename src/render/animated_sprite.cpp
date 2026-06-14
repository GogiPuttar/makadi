// src/render/animated_sprite.cpp
#include "makadi/render/animated_sprite.hpp"

#include <QDir>
#include <QStringList>

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace makadi::render {

bool AnimatedSprite::loadFromFolder(const QString& folder_path)
{
  frames_.clear();
  animation_time_sec_ = 0.0;
  current_frame_index_ = 0;

  QDir dir(folder_path);

  if (!dir.exists()) {
    return false;
  }

  const QStringList filters = {
    "*.png",
    "*.jpg",
    "*.jpeg",
    "*.bmp",
    "*.webp"
  };

  const QFileInfoList files = dir.entryInfoList(
    filters,
    QDir::Files,
    QDir::Name);

  for (const QFileInfo& file : files) {
    QPixmap pixmap;

    if (pixmap.load(file.absoluteFilePath())) {
      frames_.push_back(std::move(pixmap));
    }
  }

  return !frames_.empty();
}

bool AnimatedSprite::empty() const
{
  return frames_.empty();
}

int AnimatedSprite::frameCount() const
{
  return static_cast<int>(frames_.size());
}

const QPixmap& AnimatedSprite::frameAt(double speed, double dt_sec)
{
  if (frames_.empty()) {
    throw std::runtime_error("AnimatedSprite has no frames");
  }

  const double unclamped_fps = speed * speed_to_fps_;
  const double fps = std::clamp(unclamped_fps, min_fps_, max_fps_);

  animation_time_sec_ += dt_sec;

  current_frame_index_ =
    static_cast<int>(std::floor(animation_time_sec_ * fps)) %
    static_cast<int>(frames_.size());

  return frames_.at(current_frame_index_);
}

const QPixmap& AnimatedSprite::currentFrame() const
{
  if (frames_.empty()) {
    throw std::runtime_error("AnimatedSprite has no frames");
  }

  return frames_.at(current_frame_index_);
}

bool AnimatedSprite::hasCurrentFrame() const
{
  return !frames_.empty();
}

}  // namespace makadi::render