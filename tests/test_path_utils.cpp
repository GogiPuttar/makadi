#include "makadi/config/path_utils.hpp"

#include <gtest/gtest.h>

#include <filesystem>

TEST(PathUtils, KeepsAbsolutePath)
{
  const auto resolved = makadi::config::resolvePath(
    "/home/adityanair/makadi/configs/default.yaml",
    "/tmp/spider.png");

  EXPECT_EQ(resolved, std::filesystem::path("/tmp/spider.png"));
}

TEST(PathUtils, ResolvesRelativeToConfigDirectory)
{
  const auto resolved = makadi::config::resolvePath(
    "/home/adityanair/makadi/configs/default.yaml",
    "../assets/spider.png");

  EXPECT_EQ(
    resolved,
    std::filesystem::path("/home/adityanair/makadi/assets/spider.png"));
}

TEST(PathUtils, NormalizesDotSegments)
{
  const auto resolved = makadi::config::resolvePath(
    "/home/adityanair/makadi/configs/default.yaml",
    "./../assets/./spider.png");

  EXPECT_EQ(
    resolved,
    std::filesystem::path("/home/adityanair/makadi/assets/spider.png"));
}