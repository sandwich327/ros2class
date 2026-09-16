#include "rclcpp/rclcpp.hpp"
#include "msg_pkg/srv/add_two_strings.hpp"

class string_server : public rclcpp::Node
{
private:
    rclcpp::Service<msg_pkg::srv::AddTwoStrings>::SharedPtr service;
    void callback(const std::shared_ptr<msg_pkg::srv::AddTwoStrings::Request> request,
                  std::shared_ptr<msg_pkg::srv::AddTwoStrings::Response> response)
    {
        response->result = request->a + request->b;
    }

public:
    string_server() : rclcpp::Node("string_server")
    {
        RCLCPP_INFO(this->get_logger(), "string server initialized");
        service = this->create_service<msg_pkg::srv::AddTwoStrings>("add_two_strings",
                                                                    std::bind(&string_server::callback, this,
                                                                    std::placeholders::_1, std::placeholders::_2));
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<string_server>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
