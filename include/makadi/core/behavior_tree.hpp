// include/makadi/core/behavior_tree.hpp
#pragma once

namespace makadi::core {

enum class BehaviorStatus {
  Running,
  Success,
  Failure
};

class BehaviorNode {
public:
  virtual ~BehaviorNode() = default;
  virtual BehaviorStatus tick(double dt_sec) = 0;
};

}  // namespace makadi::core