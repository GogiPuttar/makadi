// src/config/path_utils.cpp

#include "makadi/config/path_utils.hpp"

namespace makadi::config {

std::filesystem::path resolvePath(
  const std::filesystem::path& config_path,
  const std::filesystem::path& user_path)
{
  if (user_path.is_absolute()) {
    return user_path.lexically_normal();
  }

  return (config_path.parent_path() / user_path).lexically_normal();
}

}