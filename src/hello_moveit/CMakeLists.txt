cmake_minimum_required(VERSION 3.8)
project(hello_moveit)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(moveit_ros_planning_interface REQUIRED)
find_package(rclcpp REQUIRED)
find_package(graph_msgs REQUIRED)
find_package(rviz_visual_tools REQUIRED)
find_package(moveit_visual_tools REQUIRED)

add_executable(hello_moveit src/hello_moveit.cpp)
add_executable(rviz_moveit src/rviz_moveit.cpp)
add_executable(object_moveit src/object_moveit.cpp)

ament_target_dependencies(
  hello_moveit
  "moveit_ros_planning_interface"
  "rclcpp"
)
ament_target_dependencies(
  rviz_moveit
  "moveit_ros_planning_interface"
  "rclcpp"
  "graph_msgs"
  "moveit_visual_tools"
  "rviz_visual_tools"
)
ament_target_dependencies(
  object_moveit
  "moveit_ros_planning_interface"
  "rclcpp"
  "graph_msgs"
  "moveit_visual_tools"
  "rviz_visual_tools"
)

install(TARGETS hello_moveit rviz_moveit object_moveit
  DESTINATION lib/${PROJECT_NAME})

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
