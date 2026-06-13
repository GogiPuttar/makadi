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

struct AppConfig {
  AssetConfig asset;
};

AppConfig loadAppConfig(const QString& path);

}  // namespace makadi::config