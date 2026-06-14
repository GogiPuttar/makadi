find_package(GTest REQUIRED)

add_executable(makadi_tests
  test_angle.cpp
  test_pose2d.cpp
  test_path_utils.cpp
  test_app_config.cpp
  test_behaviors.cpp

  ../src/config/app_config.cpp
  ../src/config/path_utils.cpp
  ../src/behaviors/flee_from_pointer.cpp
  ../src/behaviors/flee_from_pointer_and_turn_away.cpp
)

target_include_directories(makadi_tests PRIVATE
  ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(makadi_tests PRIVATE
  GTest::gtest_main
  Qt6::Widgets
  yaml-cpp
)

include(GoogleTest)
gtest_discover_tests(makadi_tests)