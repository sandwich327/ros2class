#include "rclcpp/rclcpp.hpp"
#include "msg_pkg/srv/add_two_strings.hpp"

class string_client : public rclcpp::Node
{
private:
    rclcpp::Client<msg_pkg::srv::AddTwoStrings>::SharedPtr client;

public:
    string_client() : rclcpp::Node("string_client")
    {
        RCLCPP_INFO(this->get_logger(), "string client initialized");
        client = this->create_client<msg_pkg::srv::AddTwoStrings>("add_two_strings");
    }

    void SendRequest(std::string a, std::string b)
    {
        if (!client)
        {
            RCLCPP_ERROR(this->get_logger(), "Client is not initialized!");
            return;
        }
        auto request = std::make_shared<msg_pkg::srv::AddTwoStrings::Request>();
        request->a = a;
        request->b = b;

        while (!client->wait_for_service(std::chrono::seconds(1)))
        {
            if (!rclcpp::ok())
            {
                RCLCPP_ERROR(this->get_logger(), "Interrupted while waiting for the service. Exiting.");
                return;
            }
            RCLCPP_INFO(this->get_logger(), "service not available, waiting again...");
        }

        auto result = client->async_send_request(request);
        // Wait for the result.
        if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), result) == rclcpp::FutureReturnCode::SUCCESS)
        {
            RCLCPP_INFO(this->get_logger(), "Add two strings: %s", result.get()->result.c_str());
        }
        else
        {
            RCLCPP_ERROR(this->get_logger(), "Failed to call service add_three_ints"); // CHANGE
        }
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    if (argc < 3)
    {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "usage: myclient X Y");
        return 1;
    }
    auto node = std::make_shared<string_client>();
    node->SendRequest(argv[1], argv[2]);
    rclcpp::shutdown();
    return 0;
}
