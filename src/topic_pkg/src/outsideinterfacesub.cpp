#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "msg_pkg/msg/student.hpp"

class outsidetopicsubnode : public rclcpp::Node
{
private:
    /* data */
    rclcpp::Subscription<msg_pkg::msg::Student>::SharedPtr subscriber;
    void Callback(msg_pkg::msg::Student::SharedPtr message)
    {
        RCLCPP_INFO(this->get_logger(), "received: %s age %d height %f", message->name.c_str(), message->age, message->height);
    }

public:
    outsidetopicsubnode() : rclcpp::Node("outsidetopicsubnode")
    {
        RCLCPP_INFO(this->get_logger(), "initialized");
        subscriber = this->create_subscription<msg_pkg::msg::Student>("studenttopic", 10, std::bind(&outsidetopicsubnode::Callback, this, std::placeholders::_1));
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<outsidetopicsubnode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
}