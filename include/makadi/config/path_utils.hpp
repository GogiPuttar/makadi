// include/makadi/config/path_utils.hpp
#pragma once

#include <filesystem>

namespace makadi::config {

std::filesystem::path resolvePath(
  const std::filesystem::path& config_path,
  const std::filesystem::path& user_path);

}