cmake_minimum_required(VERSION 3.8)
project(srv_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_srvs REQUIRED)
find_package(msg_pkg REQUIRED)
#find_package(rosidl_default_generators REQUIRED)

# rosidl_generate_interfaces(${PROJECT_NAME}
#   "srv/AddTwoInts.srv"
# )

#ament_export_dependencies(rosidl_default_runtime)

add_executable(myserver src/myserver.cpp)
add_executable(myclient src/myclient.cpp)
add_executable(mystringserver src/mystringserver.cpp)
add_executable(mystringclient src/mystringclient.cpp)
# rosidl_get_typesupport_target(cpp_typesupport_target ${PROJECT_NAME} "rosidl_typesupport_cpp")
# target_link_libraries(myserver ${cpp_typesupport_target})
# target_link_libraries(myclient ${cpp_typesupport_target})

# target_include_directories(myserver PUBLIC
#   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#   $<INSTALL_INTERFACE:include>)
# target_compile_features(myserver PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17

ament_target_dependencies(
  myserver
  rclcpp
  std_srvs
  msg_pkg
)
ament_target_dependencies(
  myclient
  rclcpp
  std_srvs
  msg_pkg
)
ament_target_dependencies(
  mystringserver
  rclcpp
  std_srvs
  msg_pkg
)
ament_target_dependencies(
  mystringclient
  rclcpp
  std_srvs
  msg_pkg
)

install(TARGETS myserver
  DESTINATION lib/${PROJECT_NAME})

install(TARGETS myclient
  DESTINATION lib/${PROJECT_NAME})

install(TARGETS mystringserver
  DESTINATION lib/${PROJECT_NAME})

install(TARGETS mystringclient
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
