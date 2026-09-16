cmake_minimum_required(VERSION 3.8)
project(topic_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
#user defined msg in other pkg used
find_package(msg_pkg REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(rosidl_default_generators REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/Ball.msg"
  DEPENDENCIES geometry_msgs)

add_executable(publisher_node src/publisher_node.cpp)
add_executable(subscriber_node src/subscriber_node.cpp)
add_executable(proposedpublisher_node src/proposedpublisher_node.cpp)
add_executable(proposedsubscriber_node src/proposedsubscriber_node.cpp)
add_executable(selftopicpublisher_node src/selftopicpublisher.cpp)
add_executable(selftopicsubscriber_node src/selftopicsubscriber.cpp)
add_executable(outsideinterfacepub_node src/outsideinterfacepub.cpp)
add_executable(outsideinterfacesub_node src/outsideinterfacesub.cpp)

#user defined msg inside this pkg used
# rosidl_target_interfaces(selftopicpublisher_node
#   ${PROJECT_NAME} "rosidl_typesupport_cpp")
# rosidl_target_interfaces(selftopicsubscriber_node
#   ${PROJECT_NAME} "rosidl_typesupport_cpp")
rosidl_get_typesupport_target(cpp_typesupport_target ${PROJECT_NAME} "rosidl_typesupport_cpp")
target_link_libraries(selftopicpublisher_node ${cpp_typesupport_target})
target_link_libraries(selftopicsubscriber_node ${cpp_typesupport_target})

ament_target_dependencies(
  publisher_node
  "rclcpp"
  "std_msgs"
)
ament_target_dependencies(
  subscriber_node
  "rclcpp"
  "std_msgs"
)
ament_target_dependencies(
  proposedpublisher_node
  "rclcpp"
  "std_msgs"
)
ament_target_dependencies(
  proposedsubscriber_node
  "rclcpp"
  "std_msgs"
)
ament_target_dependencies(
  selftopicpublisher_node
  "rclcpp"
  "std_msgs"
)
ament_target_dependencies(
  selftopicsubscriber_node
  "rclcpp"
  "std_msgs"
)
ament_target_dependencies(
  outsideinterfacepub_node
  "rclcpp"
  "std_msgs"
  "msg_pkg" #need add outside msg pkg
)
ament_target_dependencies(
  outsideinterfacesub_node
  "rclcpp"
  "std_msgs"
  "msg_pkg" #need add outside msg pkg
)

install(TARGETS publisher_node
  DESTINATION lib/${PROJECT_NAME})
install(TARGETS subscriber_node
  DESTINATION lib/${PROJECT_NAME})
install(TARGETS proposedpublisher_node
  DESTINATION lib/${PROJECT_NAME})
install(TARGETS proposedsubscriber_node
  DESTINATION lib/${PROJECT_NAME})
install(TARGETS selftopicpublisher_node
  DESTINATION lib/${PROJECT_NAME})
install(TARGETS selftopicsubscriber_node
  DESTINATION lib/${PROJECT_NAME})
install(TARGETS outsideinterfacepub_node
  DESTINATION lib/${PROJECT_NAME})
install(TARGETS outsideinterfacesub_node
  DESTINATION lib/${PROJECT_NAME})
install(DIRECTORY launch 
  DESTINATION share/${PROJECT_NAME}
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
