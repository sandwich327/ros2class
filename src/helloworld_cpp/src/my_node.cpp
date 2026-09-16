#include "rclcpp/rclcpp.hpp"

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<rclcpp::Node>("my_node");
  RCLCPP_INFO(node->get_logger(), "Hello world!");
  rclcpp::Rate loop_rate(1);//rate 1 second 1 times
  while (rclcpp::ok())
  {
      RCLCPP_INFO(node->get_logger(), "Hello world!");
      loop_rate.sleep();
  }
  rclcpp::shutdown();
  
  return 0;
}
