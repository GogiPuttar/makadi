find_package(GTest REQUIRED)

add_executable(makadi_tests
  test_main.cpp
  test_angle.cpp
  test_pose2d.cpp
  test_path_utils.cpp
  test_app_config.cpp
  test_behaviors.cpp
  test_animated_sprite.cpp
)

target_include_directories(makadi_tests PRIVATE
  ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(makadi_tests PRIVATE
  GTest::gtest
  makadi_core
)

add_test(NAME makadi_tests COMMAND makadi_tests)