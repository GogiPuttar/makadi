// include/makadi/render/animated_sprite.hpp
#pragma once

#include <QPixmap>
#include <QString>

#include <vector>

namespace makadi::render {

class AnimatedSprite {
public:
  bool loadFromFolder(const QString& folder_path);

  bool empty() const;
  int frameCount() const;

  const QPixmap& frameAt(double speed, double dt_sec);

  void setSpeedToFps(double value) { speed_to_fps_ = value; }
  void setMinFps(double value) { min_fps_ = value; }
  void setMaxFps(double value) { max_fps_ = value; }

  const QPixmap& currentFrame() const;
  bool hasCurrentFrame() const;

private:
  std::vector<QPixmap> frames_;
  double animation_time_sec_ = 0.0;

  double speed_to_fps_ = 0.05;
  double min_fps_ = 4.0;
  double max_fps_ = 18.0;

  int current_frame_index_ = 0;
};

}  // namespace makadi::render