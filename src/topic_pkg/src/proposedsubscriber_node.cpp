#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
using namespace std::chrono_literals;
using rclcpp::WallRate;

class subscriber_node : public rclcpp::Node
{
private:
    /* data */
    rclcpp::TimerBase::SharedPtr tmr;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscriber;
    int count;
    void Callback(std_msgs::msg::String::SharedPtr message)
    {
        count++;
        RCLCPP_INFO(this->get_logger(), "received: %s %d times", message->data.c_str(), count);
    }

public:
    subscriber_node() : rclcpp::Node("proposedsubscriber_node")
    {
        subscriber = this->create_subscription<std_msgs::msg::String>("proposedHelloworld_topic", 10, std::bind(&subscriber_node::Callback, this, std::placeholders::_1));
        RCLCPP_INFO(this->get_logger(), "initialized");
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<subscriber_node>();
    rclcpp::spin(node);
    rclcpp::shutdown();
}
