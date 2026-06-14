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
  double flee_radius = 160.0;
  double max_speed = 500.0;
  double damping = 0.90;
  double turn_gain = 12.0;
};

struct AppConfig {
  AssetConfig asset;
  BehaviorConfig behavior;
};

AppConfig loadAppConfig(const QString& path);

}  // namespace makadi::config