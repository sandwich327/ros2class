#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "topic_pkg/msg/ball.hpp"
using namespace std::chrono_literals;
using rclcpp::WallRate;

class selfsubscriber_node : public rclcpp::Node
{
private:
    /* data */
    rclcpp::TimerBase::SharedPtr tmr;
    rclcpp::Subscription<topic_pkg::msg::Ball>::SharedPtr subscriber;
    int count;
    void Callback(topic_pkg::msg::Ball::SharedPtr message)
    {
        count++;
        RCLCPP_INFO(this->get_logger(), "received: %f %d times", message->radius, count);
    }

public:
    selfsubscriber_node() : rclcpp::Node("proposedselfsubscriber_node")
    {
        subscriber = this->create_subscription<topic_pkg::msg::Ball>("selftopic", 10, std::bind(&selfsubscriber_node::Callback, this, std::placeholders::_1));
        RCLCPP_INFO(this->get_logger(), "initialized");
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<selfsubscriber_node>();
    rclcpp::spin(node);
    rclcpp::shutdown();
}
