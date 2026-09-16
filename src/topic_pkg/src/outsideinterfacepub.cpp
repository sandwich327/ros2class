#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "msg_pkg/msg/student.hpp"
using namespace std::chrono_literals;
using rclcpp::WallRate;

class outsidetopicpubnode:public rclcpp::Node
{
private:
   /* data */
   rclcpp::TimerBase::SharedPtr tmr;
   rclcpp::Publisher<msg_pkg::msg::Student>::SharedPtr publisher;
   void tmr_callback()
   {
       auto student = msg_pkg::msg::Student();
       student.name = "hhh";
       student.height = 1.8f;
       student.age = 27;
       publisher->publish(student);
   }
public:
   outsidetopicpubnode():rclcpp::Node("outsidetopicpubnode")
   {
       RCLCPP_INFO(this->get_logger(), "initialized");
       publisher = this->create_publisher<msg_pkg::msg::Student>("studenttopic", 10);
       tmr = this->create_wall_timer(100ms, std::bind(&outsidetopicpubnode::tmr_callback, this));
   }
};

int main(int argc, char** argv)
{
   rclcpp::init(argc,argv);
   auto node = std::make_shared<outsidetopicpubnode>();
   rclcpp::spin(node);
   rclcpp::shutdown();
}
