# Humble与Foxy版本兼容性说明

本代码仓库测试环境为：Ubuntu22.04+ROS Humble

如使用Ubuntu20.04+ROS Foxy，由于部分API接口不完全兼容，需要做一下代码修改：



## learning_action_cpp功能包

![build_error](docs\images\build_error.png)

原始代码：

```C++
// 创建一个服务器收到目标之后反馈时的回调函数
void goal_response_callback(GoalHandle::SharedPtr goal_message)
{
    if (!goal_message)
    {
        RCLCPP_ERROR(this->get_logger(), "Client: Goal was rejected by server");
        rclcpp::shutdown(); // Shut down client node
    }
    else
    {
        RCLCPP_INFO(this->get_logger(), "Client: Goal accepted by server, waiting for result");
    }
}
```

需要更改为：

```C++
// 创建一个服务器收到目标之后反馈时的回调函数
void goal_response_callback(std::shared_future<GoalHandle::SharedPtr> goal_message)
{
    if (!goal_message.get())
    {
        RCLCPP_ERROR(this->get_logger(), "Client: Goal was rejected by server");
        rclcpp::shutdown(); // Shut down client node
    }
    else
    {
        RCLCPP_INFO(this->get_logger(), "Client: Goal accepted by server, waiting for result");
    }
}
```

![source_change](docs\images\source_change.png)
