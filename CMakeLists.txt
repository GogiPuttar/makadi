cmake_minimum_required(VERSION 3.22)

project(makadi VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(X11 REQUIRED)
find_package(yaml-cpp REQUIRED)

add_executable(makadi
  src/main.cpp
  src/core/world.cpp
  src/render/overlay_window.cpp
  src/behaviors/flee_from_pointer.cpp
  src/platform/x11_overlay_platform.cpp
  src/input/pointer_provider.cpp
  src/config/app_config.cpp
  src/config/path_utils.cpp
)

target_include_directories(makadi PRIVATE
  include
  ${X11_INCLUDE_DIR}
)

target_link_libraries(makadi PRIVATE
  Qt6::Widgets
  yaml-cpp
  ${X11_LIBRARIES}
  ${X11_Xfixes_LIB}
  ${X11_Xext_LIB}
)

install(TARGETS makadi
  RUNTIME DESTINATION bin
)