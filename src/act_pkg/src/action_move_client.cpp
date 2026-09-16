#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "msg_pkg/action/move_circle.hpp"

using namespace std::chrono_literals;
using rclcpp::WallRate;
using namespace std::placeholders;

class client_node : public rclcpp::Node
{
private:
    /* data */
    rclcpp_action::Client<msg_pkg::action::MoveCircle>::SharedPtr action_client;

    // 处理目标发送后的反馈
    void goal_response_callback(const std::shared_ptr<rclcpp_action::ClientGoalHandle<msg_pkg::action::MoveCircle>> goal_handle)
    {
        if (!goal_handle)
        {
            RCLCPP_ERROR(this->get_logger(), "目标请求被服务器拒绝！"); // 输出错误信息，表示目标请求被拒绝
        }
        else
        {
            RCLCPP_INFO(this->get_logger(), "目标被接收，等待结果中"); // 输出信息，表示目标已经被接收
        }
    }

    // 处理连续反馈；
    void feedback_callback(const std::shared_ptr<rclcpp_action::ClientGoalHandle<msg_pkg::action::MoveCircle>> goal_handle, const std::shared_ptr<const msg_pkg::action::MoveCircle::Feedback> feedback)
    {
        (void)goal_handle;
        int32_t state = (int32_t)(feedback->state);             // 计算并获取反馈中的进度百分比
        RCLCPP_INFO(this->get_logger(), "当前进度: %d", state); // 输出当前进度信息
    }

    // 处理最终响应。finish
    void result_callback(const rclcpp_action::ClientGoalHandle<msg_pkg::action::MoveCircle>::WrappedResult &result)
    {
        switch (result.code)
        {
        case rclcpp_action::ResultCode::SUCCEEDED:
            break;
        case rclcpp_action::ResultCode::ABORTED:
            RCLCPP_ERROR(this->get_logger(), "任务被中止"); // 输出错误信息，表示任务被中止
            return;
        case rclcpp_action::ResultCode::CANCELED:
            RCLCPP_ERROR(this->get_logger(), "任务被取消"); // 输出错误信息，表示任务被取消
            return;
        default:
            RCLCPP_ERROR(this->get_logger(), "未知异常"); // 输出错误信息，表示发生未知异常
            return;
        }

        RCLCPP_INFO(this->get_logger(), "任务执行完毕，最终结果: finish = %d", result.result->finish); // 输出任务执行完毕的最终结果
    }

public:
    client_node() : rclcpp::Node("action_client_node")
    {
        RCLCPP_INFO(this->get_logger(), "client initialized");
        this->action_client = rclcpp_action::create_client<msg_pkg::action::MoveCircle>(this, "movecircle"); // 创建动作的客户端
    }

    // 发送请求；
    void send_goal()
    {
        if (!this->action_client)
        {
            RCLCPP_ERROR(this->get_logger(), "动作客户端未被初始化。"); // 如果客户端未初始化，输出错误信息
        }

        if (!this->action_client->wait_for_action_server(std::chrono::seconds(10)))
        {
            RCLCPP_ERROR(this->get_logger(), "服务连接失败！"); // 等待服务端连接超时，输出错误信息
            return;
        }

        auto goal_msg = msg_pkg::action::MoveCircle::Goal(); // 创建一个 Progress 动作的目标消息对象
        goal_msg.enable = true;                              // 设置目标消息中的数值字段

        RCLCPP_INFO(this->get_logger(), "发送请求数据！"); // 输出信息，表示正在发送请求

        auto send_goal_options = rclcpp_action::Client<msg_pkg::action::MoveCircle>::SendGoalOptions(); // 创建发送目标的选项对象
        send_goal_options.goal_response_callback = std::bind(&client_node::goal_response_callback, this, _1);
        send_goal_options.feedback_callback = std::bind(&client_node::feedback_callback, this, _1, _2);
        send_goal_options.result_callback = std::bind(&client_node::result_callback, this, _1);

        auto goal_handle_future = this->action_client->async_send_goal(goal_msg, send_goal_options); // 异步发送目标
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<client_node>();
    node->send_goal();
    rclcpp::spin(node);
    rclcpp::shutdown();
}