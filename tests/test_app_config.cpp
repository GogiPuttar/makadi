#include "makadi/config/app_config.hpp"

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

namespace {

std::filesystem::path writeTempConfig(const std::string& body)
{
  const auto path =
    std::filesystem::temp_directory_path() /
    ("makadi_test_config_" + std::to_string(::testing::UnitTest::GetInstance()->random_seed()) + ".yaml");

  std::ofstream file(path);
  file << body;
  return path;
}

}

TEST(AppConfig, ParsesCircleAssetDefaults)
{
  const auto path = writeTempConfig(R"(
asset:
  type: circle
)");

  const auto config =
    makadi::config::loadAppConfig(QString::fromStdString(path.string()));

  EXPECT_EQ(config.asset.type, makadi::config::AssetType::Circle);
  EXPECT_DOUBLE_EQ(config.asset.radius, 24.0);
  EXPECT_EQ(config.asset.color.toStdString(), "black");
}

TEST(AppConfig, ParsesCircleAssetFields)
{
  const auto path = writeTempConfig(R"(
asset:
  type: circle
  radius: 32
  color: red
)");

  const auto config =
    makadi::config::loadAppConfig(QString::fromStdString(path.string()));

  EXPECT_EQ(config.asset.type, makadi::config::AssetType::Circle);
  EXPECT_DOUBLE_EQ(config.asset.radius, 32.0);
  EXPECT_EQ(config.asset.color.toStdString(), "red");
}

TEST(AppConfig, ParsesImageAssetAndResolvesRelativePath)
{
  const auto config_path = writeTempConfig(R"(
asset:
  type: image
  path: ../assets/spider.png
  radius: 48
)");

  const auto config =
    makadi::config::loadAppConfig(QString::fromStdString(config_path.string()));

  EXPECT_EQ(config.asset.type, makadi::config::AssetType::Image);
  EXPECT_DOUBLE_EQ(config.asset.radius, 48.0);

  const auto expected =
    (config_path.parent_path() / "../assets/spider.png").lexically_normal();

  EXPECT_EQ(config.asset.path.toStdString(), expected.string());
}

TEST(AppConfig, ThrowsOnMissingAsset)
{
  const auto path = writeTempConfig(R"(
behavior:
  type: flee_from_pointer
)");

  EXPECT_THROW(
    makadi::config::loadAppConfig(QString::fromStdString(path.string())),
    std::runtime_error);
}

TEST(AppConfig, ThrowsOnUnknownAssetType)
{
  const auto path = writeTempConfig(R"(
asset:
  type: triangle
)");

  EXPECT_THROW(
    makadi::config::loadAppConfig(QString::fromStdString(path.string())),
    std::runtime_error);
}

TEST(AppConfig, ThrowsOnImageWithoutPath)
{
  const auto path = writeTempConfig(R"(
asset:
  type: image
)");

  EXPECT_THROW(
    makadi::config::loadAppConfig(QString::fromStdString(path.string())),
    std::runtime_error);
}

TEST(AppConfig, ParsesDefaultBehavior)
{
  const auto path = writeTempConfig(R"(
asset:
  type: circle
)");

  const auto config =
    makadi::config::loadAppConfig(QString::fromStdString(path.string()));

  EXPECT_EQ(config.behavior.type, makadi::config::BehaviorType::FleeFromPointer);
  EXPECT_DOUBLE_EQ(config.behavior.flee_radius, 160.0);
  EXPECT_DOUBLE_EQ(config.behavior.max_speed, 500.0);
  EXPECT_DOUBLE_EQ(config.behavior.damping, 0.90);
  EXPECT_DOUBLE_EQ(config.behavior.turn_gain, 12.0);
}

TEST(AppConfig, ParsesFleeBehavior)
{
  const auto path = writeTempConfig(R"(
asset:
  type: circle

behavior:
  type: flee_from_pointer
  flee_radius: 200
  max_speed: 300
  damping: 0.8
)");

  const auto config =
    makadi::config::loadAppConfig(QString::fromStdString(path.string()));

  EXPECT_EQ(config.behavior.type, makadi::config::BehaviorType::FleeFromPointer);
  EXPECT_DOUBLE_EQ(config.behavior.flee_radius, 200.0);
  EXPECT_DOUBLE_EQ(config.behavior.max_speed, 300.0);
  EXPECT_DOUBLE_EQ(config.behavior.damping, 0.8);
}

TEST(AppConfig, ParsesTurnAwayBehavior)
{
  const auto path = writeTempConfig(R"(
asset:
  type: circle

behavior:
  type: flee_from_pointer_and_turn_away
  flee_radius: 210
  max_speed: 310
  damping: 0.75
  turn_gain: 5.5
)");

  const auto config =
    makadi::config::loadAppConfig(QString::fromStdString(path.string()));

  EXPECT_EQ(config.behavior.type, makadi::config::BehaviorType::FleeFromPointerAndTurnAway);
  EXPECT_DOUBLE_EQ(config.behavior.flee_radius, 210.0);
  EXPECT_DOUBLE_EQ(config.behavior.max_speed, 310.0);
  EXPECT_DOUBLE_EQ(config.behavior.damping, 0.75);
  EXPECT_DOUBLE_EQ(config.behavior.turn_gain, 5.5);
}

TEST(AppConfig, ThrowsOnUnknownBehaviorType)
{
  const auto path = writeTempConfig(R"(
asset:
  type: circle

behavior:
  type: teleport
)");

  EXPECT_THROW(
    makadi::config::loadAppConfig(QString::fromStdString(path.string())),
    std::runtime_error);
}