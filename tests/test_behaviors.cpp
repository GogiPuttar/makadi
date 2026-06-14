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