#include <memory>
#include <rclcpp/rclcpp.hpp>
#include "moveit/move_group_interface/move_group_interface.h"

int main(int argc, char *argv[])
{
  // // Initialize ROS and create the Node
  // rclcpp::init(argc, argv);
  // auto const node = std::make_shared<rclcpp::Node>(
  //     "hello_moveit", rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true));

  // // Create a ROS logger
  // auto const logger = rclcpp::get_logger("hello_moveit");

  // // Create the MoveIt MoveGroup Interface
  // using moveit::planning_interface::MoveGroupInterface;
  // auto move_group_interface = MoveGroupInterface(node, "panda_arm");
  // 1. 初始化 ROS 并创建节点
  rclcpp::init(argc, argv);
  auto const node = std::make_shared<rclcpp::Node>(
    "hello_moveit",
    rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true)
  );
  auto const logger = rclcpp::get_logger("hello_moveit");
  // 2. 创建一个后台线程来 spin 节点（这是解决卡顿的关键！）
  rclcpp::executors::SingleThreadedExecutor executor;
  executor.add_node(node);
  auto spinner = std::thread([&executor]() { executor.spin(); });

  // 3. 创建 MoveIt MoveGroup 接口
  // 因为后台线程已经在处理通信了，这里就不会再卡住了
  using moveit::planning_interface::MoveGroupInterface;
  auto move_group_interface = MoveGroupInterface(node, "panda_arm");

  // Set a target Pose     C++ lambda
  auto const target_pose = []
  {
    geometry_msgs::msg::Pose msg;
    msg.orientation.w = 1.0;
    msg.position.x = 0.28;
    msg.position.y = -0.2;
    msg.position.z = 0.5;
    return msg;
  }();
  move_group_interface.setPoseTarget(target_pose);

  // Create a plan to that target pose
  auto const [success, plan] = [&move_group_interface]
  {
    moveit::planning_interface::MoveGroupInterface::Plan msg;
    auto const ok = static_cast<bool>(move_group_interface.plan(msg));
    return std::make_pair(ok, msg);
  }();

  // Execute the plan
  if (success)
  {
    move_group_interface.execute(plan);
  }
  else
  {
    RCLCPP_ERROR(logger, "Planning failed!");
  }

  // Shutdown ROS
  rclcpp::shutdown();
  spinner.join();  // 必须 join 线程，否则程序退出时会崩溃
  return 0;
}
