#include "rclcpp/rclcpp.hpp"
#include "msg_pkg/action/move_circle.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
using namespace std::chrono_literals;
using rclcpp::WallRate;

class server_node : public rclcpp::Node
{
private:
  rclcpp_action::Server<msg_pkg::action::MoveCircle>::SharedPtr action_server;

  // 处理请求goal数据
  rclcpp_action::GoalResponse handle_goal(const rclcpp_action::GoalUUID &uuid, std::shared_ptr<const msg_pkg::action::MoveCircle::Goal> goal)
  {
    (void)uuid;                                                                         // 不用的参数防止警告
    RCLCPP_INFO(this->get_logger(), "接收到动作客户端请求");     // 输出接收到的请求信息
    if(goal->enable == false)                      //enable = false,is not to move circle
    {
      return rclcpp_action::GoalResponse::REJECT;
    }
    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;                             // 接受并执行请求
  }

  // 处理取消任务请求
  rclcpp_action::CancelResponse handle_cancel(const std::shared_ptr<rclcpp_action::ServerGoalHandle<msg_pkg::action::MoveCircle>> goal_handle)
  {
    (void)goal_handle;                                  // 防止未使用的参数警告
    RCLCPP_INFO(this->get_logger(), "接收到任务取消请求"); // 输出任务取消请求信息
    return rclcpp_action::CancelResponse::ACCEPT;
  }

  // 执行任务
  void execute(const std::shared_ptr<rclcpp_action::ServerGoalHandle<msg_pkg::action::MoveCircle>> goal_handle)
  {
    RCLCPP_INFO(this->get_logger(), "开始执行任务");        // 输出任务开始执行任务
    rclcpp::Rate loop_rate(10.0);                           // 设置循环频率为10HZ
    const auto goal = goal_handle->get_goal();              // 获取目标信息
    auto feedback = std::make_shared<msg_pkg::action::MoveCircle::Feedback>(); // 创建反馈信息的共享指针
    auto result = std::make_shared<msg_pkg::action::MoveCircle::Result>();     // 创建结果消息的共享指针
 
    for (int i = 0; (i < 360) && rclcpp::ok(); i++) //move a circle
    {
      //检查是否有效取消请求
      if (goal_handle->is_canceling())
      {
        result->finish = false;                          // 设置结果的求和值
        goal_handle->canceled(result);               // 发布取消结果
        RCLCPP_INFO(this->get_logger(), "任务取消");  // 输出任务取消的信息
        return;                                      // 退出任务执行
      }

      feedback->state = i;                                 // 计算反馈的进度
      goal_handle->publish_feedback(feedback);                                      // 发布反馈消息
      RCLCPP_INFO(this->get_logger(), "now at %d degrees", feedback->state); // 输出反馈进度信息

      loop_rate.sleep(); // 循环休眠，控制频率
    }

    if (rclcpp::ok())
    {
      result->finish = true;                             // 设置结果和求和值
      goal_handle->succeed(result);                     // 发布任务成功的结果
      RCLCPP_INFO(this->get_logger(), "任务完成！");      // 输出任务完成的信息
    }
  }

  // 处理目标接受
  void handle_accepted(const std::shared_ptr<rclcpp_action::ServerGoalHandle<msg_pkg::action::MoveCircle>> goal_handle)
  {
    std::thread{std::bind(&server_node::execute, this, std::placeholders::_1), goal_handle}.detach(); // 创建新线程执行任务
  }

public:
  server_node() : rclcpp::Node("action_server_node")
  {
    RCLCPP_INFO(this->get_logger(), "server initialized");
    this->action_server = rclcpp_action::create_server<msg_pkg::action::MoveCircle>(this, "movecircle",
      std::bind(&server_node::handle_goal, this, std::placeholders::_1, std::placeholders::_2),
      std::bind(&server_node::handle_cancel, this, std::placeholders::_1),
      std::bind(&server_node::handle_accepted, this, std::placeholders::_1));
  }
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<server_node>();
  rclcpp::spin(node);
  rclcpp::shutdown();
}