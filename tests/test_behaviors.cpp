#include "makadi/behaviors/flee_from_pointer.hpp"
#include "makadi/behaviors/flee_from_pointer_and_turn_away.hpp"

#include "mock_pointer_provider.hpp"

#include <gtest/gtest.h>

#include <cmath>

TEST(FleeFromPointer, DoesNothingOutsideFleeRadius)
{
  makadi::core::Entity entity;
  entity.pose.x = 0.0;
  entity.pose.y = 0.0;

  MockPointerProvider pointer;
  pointer.pointer = QPointF(1000.0, 0.0);

  makadi::behaviors::FleeFromPointer behavior(entity, pointer);
  behavior.setFleeRadius(100.0);
  behavior.tick(0.1);

  EXPECT_DOUBLE_EQ(entity.pose.x, 0.0);
  EXPECT_DOUBLE_EQ(entity.pose.y, 0.0);
  EXPECT_DOUBLE_EQ(entity.velocity.x(), 0.0);
  EXPECT_DOUBLE_EQ(entity.velocity.y(), 0.0);
}

TEST(FleeFromPointer, MovesAwayFromPointer)
{
  makadi::core::Entity entity;
  entity.pose.x = 100.0;
  entity.pose.y = 0.0;

  MockPointerProvider pointer;
  pointer.pointer = QPointF(0.0, 0.0);

  makadi::behaviors::FleeFromPointer behavior(entity, pointer);
  behavior.setFleeRadius(200.0);
  behavior.setMaxSpeed(100.0);
  behavior.setDamping(1.0);

  behavior.tick(1.0);

  EXPECT_GT(entity.pose.x, 100.0);
  EXPECT_NEAR(entity.pose.y, 0.0, 1e-12);
  EXPECT_GT(entity.velocity.x(), 0.0);
}

TEST(FleeFromPointer, MovesAwayInNegativeDirection)
{
  makadi::core::Entity entity;
  entity.pose.x = -100.0;
  entity.pose.y = 0.0;

  MockPointerProvider pointer;
  pointer.pointer = QPointF(0.0, 0.0);

  makadi::behaviors::FleeFromPointer behavior(entity, pointer);
  behavior.setFleeRadius(200.0);
  behavior.setMaxSpeed(100.0);
  behavior.setDamping(1.0);

  behavior.tick(1.0);

  EXPECT_LT(entity.pose.x, -100.0);
  EXPECT_NEAR(entity.pose.y, 0.0, 1e-12);
  EXPECT_LT(entity.velocity.x(), 0.0);
}

TEST(FleeFromPointer, IgnoresZeroDistance)
{
  makadi::core::Entity entity;
  entity.pose.x = 50.0;
  entity.pose.y = 50.0;

  MockPointerProvider pointer;
  pointer.pointer = QPointF(50.0, 50.0);

  makadi::behaviors::FleeFromPointer behavior(entity, pointer);
  behavior.tick(1.0);

  EXPECT_DOUBLE_EQ(entity.pose.x, 50.0);
  EXPECT_DOUBLE_EQ(entity.pose.y, 50.0);
}

TEST(FleeFromPointer, DampingReducesVelocity)
{
  makadi::core::Entity entity;
  entity.pose.x = 100.0;
  entity.pose.y = 0.0;
  entity.velocity = QPointF(10.0, 0.0);

  MockPointerProvider pointer;
  pointer.pointer = QPointF(1000.0, 0.0);

  makadi::behaviors::FleeFromPointer behavior(entity, pointer);
  behavior.setFleeRadius(100.0);
  behavior.setDamping(0.5);

  behavior.tick(1.0);

  EXPECT_DOUBLE_EQ(entity.velocity.x(), 5.0);
  EXPECT_DOUBLE_EQ(entity.pose.x, 105.0);
}

TEST(FleeFromPointerAndTurnAway, MovesAwayAndTurnsAway)
{
  makadi::core::Entity entity;
  entity.pose.x = 100.0;
  entity.pose.y = 0.0;
  entity.pose.theta = makadi::core::Angle::fromRadians(0.0);

  MockPointerProvider pointer;
  pointer.pointer = QPointF(0.0, 0.0);

  makadi::behaviors::FleeFromPointerAndTurnAway behavior(entity, pointer);
  behavior.setFleeRadius(200.0);
  behavior.setMaxSpeed(100.0);
  behavior.setDamping(1.0);
  behavior.setTurnGain(1.0);

  behavior.tick(1.0);

  EXPECT_GT(entity.pose.x, 100.0);
  EXPECT_NEAR(entity.pose.theta.radians(), 0.0, 1e-12);
}

TEST(FleeFromPointerAndTurnAway, TurnsTowardAwayHeading)
{
  makadi::core::Entity entity;
  entity.pose.x = 0.0;
  entity.pose.y = 100.0;
  entity.pose.theta = makadi::core::Angle::fromRadians(0.0);

  MockPointerProvider pointer;
  pointer.pointer = QPointF(0.0, 0.0);

  makadi::behaviors::FleeFromPointerAndTurnAway behavior(entity, pointer);
  behavior.setFleeRadius(200.0);
  behavior.setMaxSpeed(0.0);
  behavior.setDamping(1.0);
  behavior.setTurnGain(0.5);

  behavior.tick(1.0);

  EXPECT_GT(entity.pose.theta.radians(), 0.0);
  EXPECT_LT(entity.pose.theta.radians(), makadi::core::kPi / 2.0);
}

TEST(FleeFromPointerAndTurnAway, DoesNotTurnOutsideRadius)
{
  makadi::core::Entity entity;
  entity.pose.x = 1000.0;
  entity.pose.y = 0.0;
  entity.pose.theta = makadi::core::Angle::fromRadians(0.0);

  MockPointerProvider pointer;
  pointer.pointer = QPointF(0.0, 0.0);

  makadi::behaviors::FleeFromPointerAndTurnAway behavior(entity, pointer);
  behavior.setFleeRadius(100.0);
  behavior.setTurnGain(10.0);

  behavior.tick(1.0);

  EXPECT_DOUBLE_EQ(entity.pose.theta.radians(), 0.0);
}

TEST(FleeFromPointerAndTurnAway, MinimumSpeedOnlyAppliesWhenFleeing)
{
  makadi::core::Entity entity;
  entity.pose.x = 0.0;
  entity.pose.y = 0.0;

  MockPointerProvider pointer;
  pointer.pointer = QPointF(1000.0, 0.0);

  makadi::behaviors::FleeFromPointerAndTurnAway behavior(entity, pointer);
  behavior.setFleeRadius(100.0);
  behavior.setSafeRadius(150.0);
  behavior.setMinSpeed(300.0);

  behavior.tick(1.0);

  EXPECT_DOUBLE_EQ(entity.velocity.x(), 0.0);
  EXPECT_DOUBLE_EQ(entity.velocity.y(), 0.0);
  EXPECT_DOUBLE_EQ(entity.pose.x, 0.0);
  EXPECT_DOUBLE_EQ(entity.pose.y, 0.0);
}

TEST(FleeFromPointerAndTurnAway, EnforcesMinimumSpeedWhenInsideFleeRadius)
{
  makadi::core::Entity entity;
  entity.pose.x = 100.0;
  entity.pose.y = 0.0;

  MockPointerProvider pointer;
  pointer.pointer = QPointF(0.0, 0.0);

  makadi::behaviors::FleeFromPointerAndTurnAway behavior(entity, pointer);
  behavior.setFleeRadius(200.0);
  behavior.setSafeRadius(300.0);
  behavior.setMinSpeed(300.0);
  behavior.setMaxSpeed(100.0);
  behavior.setDamping(1.0);
  behavior.setVelocityTrackingGain(100.0);

  behavior.tick(0.1);

  const double speed = std::hypot(entity.velocity.x(), entity.velocity.y());
  EXPECT_GE(speed, 299.0);
  EXPECT_GT(entity.pose.x, 100.0);
}

TEST(FleeFromPointerAndTurnAway, HysteresisKeepsFleeingUntilSafeRadius)
{
  makadi::core::Entity entity;
  entity.pose.x = 100.0;
  entity.pose.y = 0.0;

  MockPointerProvider pointer;
  pointer.pointer = QPointF(0.0, 0.0);

  makadi::behaviors::FleeFromPointerAndTurnAway behavior(entity, pointer);
  behavior.setFleeRadius(150.0);
  behavior.setSafeRadius(300.0);
  behavior.setMinSpeed(100.0);
  behavior.setMaxSpeed(100.0);
  behavior.setDamping(1.0);
  behavior.setVelocityTrackingGain(100.0);

  behavior.tick(0.1);
  EXPECT_GT(std::hypot(entity.velocity.x(), entity.velocity.y()), 0.0);

  entity.pose.x = 200.0;  // Outside flee_radius, still inside safe_radius.
  entity.velocity = QPointF(0.0, 0.0);
  behavior.tick(0.1);

  EXPECT_GT(std::hypot(entity.velocity.x(), entity.velocity.y()), 0.0);

  entity.pose.x = 400.0;  // Outside safe_radius.
  entity.velocity = QPointF(10.0, 0.0);
  behavior.tick(0.1);

  EXPECT_DOUBLE_EQ(entity.velocity.x(), 0.0);
  EXPECT_DOUBLE_EQ(entity.velocity.y(), 0.0);
}

TEST(FleeFromPointerAndTurnAway, VelocityTracksAwayDirectionInsteadOfAccumulatingOrbit)
{
  makadi::core::Entity entity;
  entity.pose.x = 100.0;
  entity.pose.y = 0.0;
  entity.velocity = QPointF(0.0, 500.0);  // Tangential/orbit-like velocity.

  MockPointerProvider pointer;
  pointer.pointer = QPointF(0.0, 0.0);

  makadi::behaviors::FleeFromPointerAndTurnAway behavior(entity, pointer);
  behavior.setFleeRadius(200.0);
  behavior.setSafeRadius(300.0);
  behavior.setMinSpeed(100.0);
  behavior.setMaxSpeed(500.0);
  behavior.setDamping(1.0);
  behavior.setVelocityTrackingGain(100.0);

  behavior.tick(0.1);

  EXPECT_GT(entity.velocity.x(), 0.0);
  EXPECT_NEAR(entity.velocity.y(), 0.0, 1e-9);
}

TEST(FleeFromPointerAndTurnAway, MaxTurnSpeedLimitsAngularStep)
{
  makadi::core::Entity entity;
  entity.pose.x = 0.0;
  entity.pose.y = 100.0;
  entity.pose.theta = makadi::core::Angle::fromRadians(0.0);

  MockPointerProvider pointer;
  pointer.pointer = QPointF(0.0, 0.0);

  makadi::behaviors::FleeFromPointerAndTurnAway behavior(entity, pointer);
  behavior.setFleeRadius(200.0);
  behavior.setSafeRadius(300.0);
  behavior.setTurnGain(100.0);
  behavior.setMaxTurnSpeed(1.0);
  behavior.setVelocityTrackingGain(100.0);

  behavior.tick(0.1);

  EXPECT_NEAR(entity.pose.theta.radians(), 0.1, 1e-9);
}

TEST(FleeFromPointerAndTurnAway, HeadingOffsetIsApplied)
{
  makadi::core::Entity entity;
  entity.pose.x = 100.0;
  entity.pose.y = 0.0;
  entity.pose.theta = makadi::core::Angle::fromRadians(0.0);

  MockPointerProvider pointer;
  pointer.pointer = QPointF(0.0, 0.0);

  makadi::behaviors::FleeFromPointerAndTurnAway behavior(entity, pointer);
  behavior.setFleeRadius(200.0);
  behavior.setSafeRadius(300.0);
  behavior.setTurnGain(1.0);
  behavior.setMaxTurnSpeed(100.0);
  behavior.setHeadingOffset(makadi::core::Angle::fromDegrees(90.0));

  behavior.tick(1.0);

  EXPECT_NEAR(entity.pose.theta.degrees(), 90.0, 1e-9);
}

TEST(FleeFromPointerAndTurnAway, PointerFilterAlphaOneUsesRawPointerImmediately)
{
  makadi::core::Entity entity;
  entity.pose.x = 100.0;
  entity.pose.y = 0.0;

  MockPointerProvider pointer;
  pointer.pointer = QPointF(0.0, 0.0);

  makadi::behaviors::FleeFromPointerAndTurnAway behavior(entity, pointer);
  behavior.setFleeRadius(200.0);
  behavior.setSafeRadius(300.0);
  behavior.setMinSpeed(100.0);
  behavior.setMaxSpeed(100.0);
  behavior.setDamping(1.0);
  behavior.setVelocityTrackingGain(100.0);
  behavior.setPointerFilterAlpha(1.0);

  behavior.tick(0.1);

  EXPECT_GT(entity.velocity.x(), 0.0);
}

TEST(FleeFromPointerAndTurnAway, PointerFilterAlphaZeroHoldsInitialPointer)
{
  makadi::core::Entity entity;
  entity.pose.x = 100.0;
  entity.pose.y = 0.0;

  MockPointerProvider pointer;
  pointer.pointer = QPointF(0.0, 0.0);

  makadi::behaviors::FleeFromPointerAndTurnAway behavior(entity, pointer);
  behavior.setFleeRadius(200.0);
  behavior.setSafeRadius(300.0);
  behavior.setMinSpeed(100.0);
  behavior.setMaxSpeed(100.0);
  behavior.setDamping(1.0);
  behavior.setVelocityTrackingGain(100.0);
  behavior.setPointerFilterAlpha(0.0);

  behavior.tick(0.1);
  const double first_vx = entity.velocity.x();

  entity.velocity = QPointF(0.0, 0.0);
  pointer.pointer = QPointF(1000.0, 0.0);

  behavior.tick(0.1);

  EXPECT_GT(first_vx, 0.0);
  EXPECT_GT(entity.velocity.x(), 0.0);
}

TEST(FleeFromPointerAndTurnAway, PointerFilterAlphaIsClamped)
{
  makadi::core::Entity entity;
  entity.pose.x = 100.0;
  entity.pose.y = 0.0;

  MockPointerProvider pointer;
  pointer.pointer = QPointF(0.0, 0.0);

  makadi::behaviors::FleeFromPointerAndTurnAway behavior(entity, pointer);

  EXPECT_NO_THROW(behavior.setPointerFilterAlpha(-10.0));
  EXPECT_NO_THROW(behavior.setPointerFilterAlpha(10.0));
}