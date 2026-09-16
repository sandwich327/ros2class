#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

int main(int argc, char ** argv)
{
  int count = 0;
  rclcpp::init(argc,argv);
  auto node = std::make_shared<rclcpp::Node>("publish_node");
  auto publisher = node->create_publisher<std_msgs::msg::String>("/helloworld_topic", 10);
  std_msgs::msg::String message;
  message.data = "Hello World";

  rclcpp::Rate loop_rate(1);
  while(rclcpp::ok())
  {
    count ++;    
    RCLCPP_INFO(node->get_logger(),"published: %s %d times", message.data.c_str(), count);
    publisher->publish(message);
    loop_rate.sleep();
  }
  rclcpp::shutdown();
  return 0;
}
