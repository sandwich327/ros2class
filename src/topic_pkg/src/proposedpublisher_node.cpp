#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
using namespace std::chrono_literals;
using rclcpp::WallRate;

class publisher_node : public rclcpp::Node
{
private:
    /* data */
    rclcpp::TimerBase::SharedPtr tmr;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher;
    int count;
    void tmr_callback()
    {
        count++;
        auto message = std_msgs::msg::String();
        message.data = "Hello World";
        RCLCPP_INFO(this->get_logger(), "published: %s %d times", message.data.c_str(), count);
        publisher->publish(message);
    }

public:
    publisher_node() : rclcpp::Node("proposedpublisher_node")
    {
        publisher = this->create_publisher<std_msgs::msg::String>("proposedHelloworld_topic", 10);
        RCLCPP_INFO(this->get_logger(), "initialized");
        tmr = this->create_wall_timer(1s, std::bind(&publisher_node::tmr_callback, this));
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<publisher_node>();
    rclcpp::spin(node);
    rclcpp::shutdown();
}
