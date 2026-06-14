#include "makadi/core/pose2d.hpp"

#include <gtest/gtest.h>

TEST(Pose2D, DefaultsToOrigin)
{
  makadi::core::Pose2D pose;

  EXPECT_DOUBLE_EQ(pose.x, 0.0);
  EXPECT_DOUBLE_EQ(pose.y, 0.0);
  EXPECT_DOUBLE_EQ(pose.theta.radians(), 0.0);
}

TEST(Pose2D, StoresNormalizedTheta)
{
  makadi::core::Pose2D pose;
  pose.theta = makadi::core::Angle::fromDegrees(270.0);

  EXPECT_NEAR(pose.theta.degrees(), -90.0, 1e-12);
}