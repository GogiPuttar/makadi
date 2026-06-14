#include "makadi/render/animated_sprite.hpp"

#include <gtest/gtest.h>

#include <QImage>
#include <QColor>
#include <QTemporaryDir>

static void writePng(const QString& path, const QColor& color)
{
  QImage image(8, 8, QImage::Format_ARGB32);
  image.fill(color);
  ASSERT_TRUE(image.save(path));
}

TEST(AnimatedSprite, FailsForMissingFolder)
{
  makadi::render::AnimatedSprite sprite;
  EXPECT_FALSE(sprite.loadFromFolder("/tmp/does_not_exist_makadi"));
  EXPECT_TRUE(sprite.empty());
  EXPECT_EQ(sprite.frameCount(), 0);
}

TEST(AnimatedSprite, LoadsFramesAlphabetically)
{
  QTemporaryDir dir;
  ASSERT_TRUE(dir.isValid());

  writePng(dir.path() + "/walk_02.png", Qt::red);
  writePng(dir.path() + "/walk_00.png", Qt::green);
  writePng(dir.path() + "/walk_01.png", Qt::blue);

  makadi::render::AnimatedSprite sprite;
  EXPECT_TRUE(sprite.loadFromFolder(dir.path()));
  EXPECT_FALSE(sprite.empty());
  EXPECT_EQ(sprite.frameCount(), 3);
}

TEST(AnimatedSprite, HoldsCurrentFrameWhenNotAdvanced)
{
  QTemporaryDir dir;
  ASSERT_TRUE(dir.isValid());

  writePng(dir.path() + "/walk_00.png", Qt::red);
  writePng(dir.path() + "/walk_01.png", Qt::green);

  makadi::render::AnimatedSprite sprite;
  ASSERT_TRUE(sprite.loadFromFolder(dir.path()));

  const auto& first = sprite.currentFrame();
  const auto& again = sprite.currentFrame();

  EXPECT_EQ(first.cacheKey(), again.cacheKey());
}

TEST(AnimatedSprite, AdvancesFrameWithSpeedAndDt)
{
  QTemporaryDir dir;
  ASSERT_TRUE(dir.isValid());

  writePng(dir.path() + "/walk_00.png", Qt::red);
  writePng(dir.path() + "/walk_01.png", Qt::green);
  writePng(dir.path() + "/walk_02.png", Qt::blue);

  makadi::render::AnimatedSprite sprite;
  ASSERT_TRUE(sprite.loadFromFolder(dir.path()));

  sprite.setSpeedToFps(1.0);
  sprite.setMinFps(0.0);
  sprite.setMaxFps(100.0);

  const auto& f0 = sprite.frameAt(1.0, 0.0);
  const auto& f1 = sprite.frameAt(1.0, 1.0);

  EXPECT_NE(f0.cacheKey(), f1.cacheKey());
}

TEST(AnimatedSprite, ThrowsWhenEmpty)
{
  makadi::render::AnimatedSprite sprite;

  EXPECT_THROW(sprite.currentFrame(), std::runtime_error);
  EXPECT_THROW(sprite.frameAt(1.0, 0.1), std::runtime_error);
}