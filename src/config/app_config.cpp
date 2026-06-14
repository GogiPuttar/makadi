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

    if (behavior["safe_radius"]) {
      config.behavior.safe_radius = behavior["safe_radius"].as<double>();
    }

    if (behavior["min_speed"]) {
      config.behavior.min_speed = behavior["min_speed"].as<double>();
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
    
    if (behavior["max_turn_speed"]) {
      config.behavior.max_turn_speed = behavior["max_turn_speed"].as<double>();
    }

    if (behavior["heading_offset_deg"]) {
      config.behavior.heading_offset_deg = behavior["heading_offset_deg"].as<double>();
    }

    if (behavior["velocity_tracking_gain"]) {
      config.behavior.velocity_tracking_gain = behavior["velocity_tracking_gain"].as<double>();
    }

    if (behavior["pointer_filter_alpha"]) {
      config.behavior.pointer_filter_alpha =
        behavior["pointer_filter_alpha"].as<double>();
    }
  }

  if (config.behavior.safe_radius <= 0.0) {
    config.behavior.safe_radius = config.behavior.flee_radius;
  }

  const YAML::Node animation = root["animation"];
  if (animation) {
    const YAML::Node walking = animation["walking"];

    if (walking) {
      if (walking["frames_folder"]) {
        const auto resolved = resolvePath(
          std::filesystem::path(path.toStdString()),
          std::filesystem::path(walking["frames_folder"].as<std::string>()));

        config.animation.walking.frames_folder =
          QString::fromStdString(resolved.string());
      }

      if (walking["speed_to_fps"]) {
        config.animation.walking.speed_to_fps =
          walking["speed_to_fps"].as<double>();
      }

      if (walking["min_fps"]) {
        config.animation.walking.min_fps =
          walking["min_fps"].as<double>();
      }

      if (walking["max_fps"]) {
        config.animation.walking.max_fps =
          walking["max_fps"].as<double>();
      }
    }
  }

  return config;
}

}  // namespace makadi::config