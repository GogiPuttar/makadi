#pragma once

#include <cmath>

namespace makadi::core {

constexpr double kPi = 3.141592653589793238462643383279502884;

class Angle {
public:
  constexpr Angle() = default;

  static constexpr Angle fromRadians(double radians)
  {
    return Angle(normalize(radians), NormalizedTag{});
  }

  static constexpr Angle fromDegrees(double degrees)
  {
    return fromRadians(degrees * kPi / 180.0);
  }

  constexpr double radians() const { return radians_; }
  constexpr double degrees() const { return radians_ * 180.0 / kPi; }

  constexpr Angle operator+() const { return *this; }
  constexpr Angle operator-() const { return fromRadians(-radians_); }

  constexpr Angle& operator+=(Angle other)
  {
    radians_ = normalize(radians_ + other.radians_);
    return *this;
  }

  constexpr Angle& operator-=(Angle other)
  {
    radians_ = normalize(radians_ - other.radians_);
    return *this;
  }

  constexpr Angle& operator*=(double scalar)
  {
    radians_ = normalize(radians_ * scalar);
    return *this;
  }

  constexpr Angle& operator/=(double scalar)
  {
    radians_ = normalize(radians_ / scalar);
    return *this;
  }

  friend constexpr Angle operator+(Angle lhs, Angle rhs)
  {
    lhs += rhs;
    return lhs;
  }

  friend constexpr Angle operator-(Angle lhs, Angle rhs)
  {
    lhs -= rhs;
    return lhs;
  }

  friend constexpr Angle operator*(Angle angle, double scalar)
  {
    angle *= scalar;
    return angle;
  }

  friend constexpr Angle operator*(double scalar, Angle angle)
  {
    angle *= scalar;
    return angle;
  }

  friend constexpr Angle operator/(Angle angle, double scalar)
  {
    angle /= scalar;
    return angle;
  }

  friend constexpr bool operator==(Angle lhs, Angle rhs)
  {
    return lhs.radians_ == rhs.radians_;
  }

  friend constexpr bool operator!=(Angle lhs, Angle rhs)
  {
    return !(lhs == rhs);
  }

  friend constexpr bool operator<(Angle lhs, Angle rhs)
  {
    return lhs.radians_ < rhs.radians_;
  }

  friend constexpr bool operator<=(Angle lhs, Angle rhs)
  {
    return lhs.radians_ <= rhs.radians_;
  }

  friend constexpr bool operator>(Angle lhs, Angle rhs)
  {
    return lhs.radians_ > rhs.radians_;
  }

  friend constexpr bool operator>=(Angle lhs, Angle rhs)
  {
    return lhs.radians_ >= rhs.radians_;
  }

private:
  struct NormalizedTag {};

  constexpr Angle(double radians, NormalizedTag)
  : radians_(radians)
  {}

  static constexpr double twoPi()
  {
    return 2.0 * kPi;
  }

  static constexpr double normalize(double radians)
  {
    while (radians <= -kPi) {
      radians += twoPi();
    }

    while (radians > kPi) {
      radians -= twoPi();
    }

    return radians;
  }

  double radians_ = 0.0;
};

}  // namespace makadi::core