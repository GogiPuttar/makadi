// src/config/app_config.cpp
#include "makadi/config/app_config.hpp"
#include "makadi/config/path_utils.hpp"

#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <stdexcept>

namespace makadi::config {

static AssetType parseAssetType(const std::string& value)
{
  if (value == "circle") {
    return AssetType::Circle;
  }

  if (value == "image") {
    return AssetType::Image;
  }

  throw std::runtime_error("Unknown asset.type: " + value);
}

static BehaviorType parseBehaviorType(const std::string& value)
{
  if (value == "flee_from_pointer") {
    return BehaviorType::FleeFromPointer;
  }

  if (value == "flee_from_pointer_and_turn_away") {
    return BehaviorType::FleeFromPointerAndTurnAway;
  }

  throw std::runtime_error("Unknown behavior.type: " + value);
}

AppConfig loadAppConfig(const QString& path)
{
  YAML::Node root = YAML::LoadFile(path.toStdString());

  AppConfig config;

  const YAML::Node asset = root["asset"];
  if (!asset) {
    throw std::runtime_error("Missing required field: asset");
  }

  if (asset["type"]) {
    config.asset.type = parseAssetType(asset["type"].as<std::string>());
  }

  if (asset["path"]) {
    const auto resolved =
      resolvePath(
        std::filesystem::path(path.toStdString()),
        std::filesystem::path(asset["path"].as<std::string>()));

    config.asset.path =
      QString::fromStdString(resolved.string());
  }

  if (asset["color"]) {
    config.asset.color = QString::fromStdString(asset["color"].as<std::string>());
  }

  if (asset["radius"]) {
    config.asset.radius = asset["radius"].as<double>();
  }

  if (config.asset.type == AssetType::Image && config.asset.path.isEmpty()) {
    throw std::runtime_error("asset.path is required when asset.type is image");
  }

  const YAML::Node behavior = root["behavior"];
  if (behavior) {
    if (behavior["type"]) {
      config.behavior.type = parseBehaviorType(behavior["type"].as<std::string>());
    }

    if (behavior["flee_radius"]) {
      config.behavior.flee_radius = behavior["flee_radius"].as<double>();
    }

    if (behavior["max_speed"]) {
      config.behavior.max_speed = behavior["max_speed"].as<double>();
    }

    if (behavior["damping"]) {
      config.behavior.damping = behavior["damping"].as<double>();
    }

    if (behavior["turn_gain"]) {
      config.behavior.turn_gain = behavior["turn_gain"].as<double>();
    }
  }

  return config;
}

}  // namespace makadi::config