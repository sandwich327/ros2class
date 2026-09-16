#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "topic_pkg/msg/ball.hpp"

using namespace std::chrono_literals;
using rclcpp::WallRate;

class selftopicpublisher_node : public rclcpp::Node
{
private:
    /* data */
    rclcpp::TimerBase::SharedPtr tmr;
    rclcpp::Publisher<topic_pkg::msg::Ball>::SharedPtr publisher;
    int count;
    void tmr_callback()
    {
        count++;
        auto message = topic_pkg::msg::Ball();
        message.radius = 10.1;
        message.center.x = 0;
        message.center.y = 0;
        message.center.z = 1;
        RCLCPP_INFO(this->get_logger(), "published: %f %d times", message.radius, count);
        publisher->publish(message);
    }

public:
    selftopicpublisher_node() : rclcpp::Node("selftopicpublisher")
    {
        publisher = this->create_publisher<topic_pkg::msg::Ball>("selftopic", 10);
        RCLCPP_INFO(this->get_logger(), "initialized");
        tmr = this->create_wall_timer(1s, std::bind(&selftopicpublisher_node::tmr_callback, this));
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<selftopicpublisher_node>();
    rclcpp::spin(node);
    rclcpp::shutdown();
}
