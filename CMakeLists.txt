cmake_minimum_required(VERSION 3.22)

project(makadi VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(makadi
  src/main.cpp
  src/core/world.cpp
  src/render/overlay_window.cpp
  src/behaviors/flee_from_pointer.cpp
)

target_include_directories(makadi PRIVATE include)

target_link_libraries(makadi PRIVATE Qt6::Widgets)

install(TARGETS makadi
  RUNTIME DESTINATION bin
)