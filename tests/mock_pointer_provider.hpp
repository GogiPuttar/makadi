#pragma once

#include "makadi/input/pointer_provider.hpp"

class MockPointerProvider final : public makadi::input::PointerProvider {
public:
  QPointF pointer;

  QPointF pointerPosition() const override
  {
    return pointer;
  }
};