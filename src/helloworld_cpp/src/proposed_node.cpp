#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
using namespace std::chrono_literals;
using rclcpp::WallRate;

class proposed_coding:public rclcpp::Node
{
private:
    /* data */
    rclcpp::TimerBase::SharedPtr tmr;
    void tmr_callback()
    {
        auto message = std_msgs::msg::String();
        message.data = "Hello World";
        RCLCPP_INFO(this->get_logger(), message.data.c_str());
    }
public:
    proposed_coding():rclcpp::Node("proposed_node")
    {
        RCLCPP_INFO(this->get_logger(), "initialized");
        //tmr = this->create_wall_timer(500ms, &proposed_coding::tmr_callback);
        tmr = this->create_wall_timer(100ms, std::bind(&proposed_coding::tmr_callback, this));
    }
};

int main(int argc, char** argv)
{
    rclcpp::init(argc,argv);
    auto node = std::make_shared<proposed_coding>();
    rclcpp::spin(node);
    rclcpp::shutdown();
}

