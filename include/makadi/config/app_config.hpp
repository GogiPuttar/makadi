// include/makadi/config/app_config.hpp
#pragma once

#include <QString>

namespace makadi::config {

enum class AssetType {
  Circle,
  Image
};

struct AssetConfig {
  AssetType type = AssetType::Circle;
  QString path;
  QString color = "black";
  double radius = 24.0;
};

enum class BehaviorType {
  FleeFromPointer,
  FleeFromPointerAndTurnAway
};

struct BehaviorConfig {
  BehaviorType type = BehaviorType::FleeFromPointer;
  double flee_radius = 60.0;
  double safe_radius = 160.0;
  double min_speed = 0.0;
  double max_speed = 500.0;
  double damping = 0.90;
  double turn_gain = 12.0;
  double max_turn_speed = 12.0;
  double heading_offset_deg = 0.0;
};

struct WalkingAnimationConfig {
  QString frames_folder;
  double speed_to_fps = 0.05;
  double min_fps = 4.0;
  double max_fps = 18.0;
};

struct AnimationConfig {
  WalkingAnimationConfig walking;
};

struct AppConfig {
  AssetConfig asset;
  BehaviorConfig behavior;
  AnimationConfig animation;
};

AppConfig loadAppConfig(const QString& path);

}  // namespace makadi::config