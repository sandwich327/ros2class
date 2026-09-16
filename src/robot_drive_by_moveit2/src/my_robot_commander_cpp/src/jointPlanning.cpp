#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.h>

class TestMoveItNode : public rclcpp::Node
{
public:
  explicit TestMoveItNode(const rclcpp::NodeOptions & options = rclcpp::NodeOptions())
  : Node("test_moveit", options), has_run_(false)
  {
    RCLCPP_INFO(this->get_logger(), "TestMoveItNode starting, waiting for parameters...");

    // 延时一点时间再做规划，确保参数已经注入
    timer_ = this->create_wall_timer(
      std::chrono::seconds(2),
      std::bind(&TestMoveItNode::timerCallback, this));
  }

private:
  void timerCallback()
  {
    if (has_run_) {
      return;  // 只执行一次
    }
    has_run_ = true;

    RCLCPP_INFO(this->get_logger(), "Timer triggered, creating MoveGroupInterface...");

    // shared_from_this return this node to the interfaces
    moveit::planning_interface::MoveGroupInterface arm(shared_from_this(), "arm");
    moveit::planning_interface::MoveGroupInterface gripper(shared_from_this(), "gripper");

    arm.setMaxVelocityScalingFactor(1.0);
    arm.setMaxAccelerationScalingFactor(1.0);

    std::vector<double> joints = { 1.5, 0.5, 0.0, 1.5, 0.0, -0.7 };

    arm.setStartStateToCurrentState();
    arm.setJointValueTarget(joints);

    moveit::planning_interface::MoveGroupInterface::Plan plan1;
    bool success1 = (arm.plan(plan1) == moveit::core::MoveItErrorCode::SUCCESS);

    if (success1) {
      RCLCPP_INFO(this->get_logger(), "Planning succeeded, executing...");
      auto result = arm.execute(plan1);
      if (result != moveit::core::MoveItErrorCode::SUCCESS) {
        RCLCPP_ERROR(this->get_logger(), "Execution failed");
      } else {
        RCLCPP_INFO(this->get_logger(), "Execution success");
      }
    } else {
      RCLCPP_ERROR(this->get_logger(), "Planning failed");
    }
  }

  rclcpp::TimerBase::SharedPtr timer_;
  bool has_run_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<TestMoveItNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}