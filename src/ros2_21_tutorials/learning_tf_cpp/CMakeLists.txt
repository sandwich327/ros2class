cmake_minimum_required(VERSION 3.8)
project(learning_tf_cpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(turtlesim REQUIRED)


add_executable(static_tf_broadcaster src/static_tf_broadcaster.cpp)
ament_target_dependencies(
   static_tf_broadcaster
   geometry_msgs
   rclcpp
   tf2
   tf2_ros
)

add_executable(turtle_tf_broadcaster src/turtle_tf_broadcaster.cpp)
ament_target_dependencies(
    turtle_tf_broadcaster
    geometry_msgs
    rclcpp
    tf2
    tf2_ros
    turtlesim
)

add_executable(tf_listener src/tf_listener.cpp)
ament_target_dependencies(
    tf_listener
    geometry_msgs
    rclcpp
    tf2
    tf2_ros
)

add_executable(turtle_following src/turtle_following.cpp)
ament_target_dependencies(
    turtle_following
    geometry_msgs
    rclcpp
    tf2
    tf2_ros
    turtlesim
)

install(TARGETS
   static_tf_broadcaster
   turtle_tf_broadcaster
   tf_listener
   turtle_following
   DESTINATION lib/${PROJECT_NAME})
   
install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}/
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
