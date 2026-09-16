cmake_minimum_required(VERSION 3.8)
project(my_robot_commander_cpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(moveit_ros_planning_interface REQUIRED)
find_package(example_interfaces REQUIRED)
find_package(my_robot_interfaces REQUIRED)

add_executable(test_moveit src/test_moveit.cpp)
ament_target_dependencies(test_moveit rclcpp moveit_ros_planning_interface)

add_executable(test_jointplan src/test_jointplan.cpp)
ament_target_dependencies(test_jointplan rclcpp moveit_ros_planning_interface)

add_executable(test_decar src/test_decar.cpp)
ament_target_dependencies(test_decar rclcpp moveit_ros_planning_interface)

add_executable(commander src/commander_template.cpp)
ament_target_dependencies(commander rclcpp moveit_ros_planning_interface example_interfaces my_robot_interfaces)

add_executable(jointPlanning src/jointPlanning.cpp)
ament_target_dependencies(jointPlanning rclcpp moveit_ros_planning_interface)

install(TARGETS
  test_moveit
  test_jointplan
  test_decar
  commander
  jointPlanning  
  DESTINATION lib/${PROJECT_NAME}/
)

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
