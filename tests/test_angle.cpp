#include "makadi/core/angle.hpp"

#include <gtest/gtest.h>

using makadi::core::Angle;
using makadi::core::kPi;

TEST(Angle, DefaultIsZero)
{
  EXPECT_DOUBLE_EQ(Angle{}.radians(), 0.0);
}

TEST(Angle, NormalizesPiBoundary)
{
  EXPECT_DOUBLE_EQ(Angle::fromRadians(kPi).radians(), kPi);
  EXPECT_DOUBLE_EQ(Angle::fromRadians(-kPi).radians(), kPi);
}

TEST(Angle, NormalizesLargePositive)
{
  EXPECT_NEAR(Angle::fromRadians(3.0 * kPi / 2.0).radians(), -kPi / 2.0, 1e-12);
  EXPECT_NEAR(Angle::fromRadians(5.0 * kPi).radians(), kPi, 1e-12);
}

TEST(Angle, NormalizesLargeNegative)
{
  EXPECT_NEAR(Angle::fromRadians(-3.0 * kPi / 2.0).radians(), kPi / 2.0, 1e-12);
  EXPECT_NEAR(Angle::fromRadians(-5.0 * kPi).radians(), kPi, 1e-12);
}

TEST(Angle, ConvertsDegrees)
{
  EXPECT_NEAR(Angle::fromDegrees(180.0).radians(), kPi, 1e-12);
  EXPECT_NEAR(Angle::fromDegrees(-180.0).radians(), kPi, 1e-12);
  EXPECT_NEAR(Angle::fromDegrees(90.0).radians(), kPi / 2.0, 1e-12);
}

TEST(Angle, AdditionNormalizes)
{
  const auto a = Angle::fromDegrees(170.0);
  const auto b = Angle::fromDegrees(30.0);

  EXPECT_NEAR((a + b).degrees(), -160.0, 1e-12);
}

TEST(Angle, SubtractionNormalizes)
{
  const auto a = Angle::fromDegrees(-170.0);
  const auto b = Angle::fromDegrees(30.0);

  EXPECT_NEAR((a - b).degrees(), 160.0, 1e-12);
}

TEST(Angle, MultiplicationNormalizes)
{
  EXPECT_NEAR((Angle::fromDegrees(100.0) * 2.0).degrees(), -160.0, 1e-12);
  EXPECT_NEAR((2.0 * Angle::fromDegrees(100.0)).degrees(), -160.0, 1e-12);
  EXPECT_NEAR((Angle::fromDegrees(90.0) * 2).degrees(), 180.0, 1e-12);
  EXPECT_NEAR((Angle::fromDegrees(90.0) * 2.0f).degrees(), 180.0, 1e-12);
}

TEST(Angle, DivisionNormalizes)
{
  EXPECT_NEAR((Angle::fromDegrees(180.0) / 2.0).degrees(), 90.0, 1e-12);
  EXPECT_NEAR((Angle::fromDegrees(180.0) / 2).degrees(), 90.0, 1e-12);
  EXPECT_NEAR((Angle::fromDegrees(180.0) / 2.0f).degrees(), 90.0, 1e-12);
}

TEST(Angle, CompoundOperatorsNormalize)
{
  auto a = Angle::fromDegrees(170.0);
  a += Angle::fromDegrees(30.0);
  EXPECT_NEAR(a.degrees(), -160.0, 1e-12);

  a -= Angle::fromDegrees(30.0);
  EXPECT_NEAR(a.degrees(), 170.0, 1e-12);

  a *= 2.0;
  EXPECT_NEAR(a.degrees(), -20.0, 1e-12);

  a /= 2.0;
  EXPECT_NEAR(a.degrees(), -10.0, 1e-12);
}

TEST(Angle, Comparisons)
{
  EXPECT_TRUE(Angle::fromDegrees(10.0) == Angle::fromDegrees(10.0));
  EXPECT_TRUE(Angle::fromDegrees(10.0) != Angle::fromDegrees(20.0));
  EXPECT_TRUE(Angle::fromDegrees(10.0) < Angle::fromDegrees(20.0));
  EXPECT_TRUE(Angle::fromDegrees(10.0) <= Angle::fromDegrees(10.0));
  EXPECT_TRUE(Angle::fromDegrees(20.0) > Angle::fromDegrees(10.0));
  EXPECT_TRUE(Angle::fromDegrees(20.0) >= Angle::fromDegrees(20.0));
}