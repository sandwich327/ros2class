/***
@作者: 古月居(www.guyuehome.com)
@说明: ROS2 TF示例-通过坐标变化实现海龟跟随功能
***/

#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "geometry_msgs/msg/transform_stamped.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"
#include "tf2/exceptions.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"
#include "turtlesim/srv/spawn.hpp"

using namespace std::chrono_literals;

class TurtleFollowing : public rclcpp::Node
{
public:
    TurtleFollowing()
        : Node("turtle_tf2_frame_listener"),
        turtle_spawning_service_ready_(false),
        turtle_spawned_(false)                    //ROS2节点父类初始化
    {
        // 创建一个目标坐标系名的参数,优先使用外部设置的参数值，否则用默认值
        target_frame_ = this->declare_parameter<std::string>("target_frame", "turtle1");

        // 创建保存坐标变换信息的缓冲区
        tf_buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());
        
        // 创建坐标变换的监听器
        tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);

        // 创建一个请求产生海龟的客户端
        spawner_ = this->create_client<turtlesim::srv::Spawn>("spawn");

        // 创建跟随运动海龟的速度话题
        publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("turtle2/cmd_vel", 1);

        // 创建一个固定周期的定时器，处理坐标信息
        timer_ = this->create_wall_timer(
            1s, std::bind(&TurtleFollowing::on_timer, this));
    }

private:
    void on_timer()
    {
        // 设置源坐标系和目标坐标系的名称
        std::string fromFrameRel = target_frame_.c_str();
        std::string toFrameRel = "turtle2";

        if (turtle_spawning_service_ready_) {
            if (turtle_spawned_) {
                geometry_msgs::msg::TransformStamped t;

                // 监听当前时刻源坐标系到目标坐标系的坐标变换
                try {
                    t = tf_buffer_->lookupTransform(
                        toFrameRel, fromFrameRel,
                        tf2::TimePointZero);
                } catch (const tf2::TransformException & ex) {
                    // 如果坐标变换获取失败，进入异常报告
                    RCLCPP_INFO(
                        this->get_logger(), "Could not transform %s to %s: %s",
                        toFrameRel.c_str(), fromFrameRel.c_str(), ex.what());
                    return;
                }
                
                // 创建速度控制消息
                geometry_msgs::msg::Twist msg;
                // 根据海龟角度，计算角速度
                static const double scaleRotationRate = 1.0;
                msg.angular.z = scaleRotationRate * atan2(
                    t.transform.translation.y,
                    t.transform.translation.x);
                    
                // 根据海龟距离，计算线速度
                static const double scaleForwardSpeed = 0.5;
                msg.linear.x = scaleForwardSpeed * sqrt(
                    pow(t.transform.translation.x, 2) +
                    pow(t.transform.translation.y, 2));

                // 发布速度指令，海龟跟随运动
                publisher_->publish(msg);
            } else {
                // 查看海龟是否生成
                RCLCPP_INFO(this->get_logger(), "Successfully spawned");
                turtle_spawned_ = true;
            }
        } else {
            // 如果海龟生成服务器已经准备就绪
            if (spawner_->service_is_ready()) {
                // 创建一个请求的数据,设置请求数据的内容，包括海龟名、xy位置、姿态
                auto request = std::make_shared<turtlesim::srv::Spawn::Request>();
                request->x = 4.0;
                request->y = 2.0;
                request->theta = 0.0;
                request->name = "turtle2";

                // 发送服务请求
                using ServiceResponseFuture =
                rclcpp::Client<turtlesim::srv::Spawn>::SharedFuture;
                auto response_received_callback = [this](ServiceResponseFuture future) {
                    auto result = future.get();
                    if (strcmp(result->name.c_str(), "turtle2") == 0) {
                        // 设置标志位，表示已经发送请求
                        turtle_spawning_service_ready_ = true;
                    } else {
                        RCLCPP_ERROR(this->get_logger(), "Service callback result mismatch");
                    }
                };
                auto result = spawner_->async_send_request(request, response_received_callback);
            } else {
                // 海龟生成服务器还没准备就绪的提示
                RCLCPP_INFO(this->get_logger(), "Service is not ready");
            }
        }
    }

    bool turtle_spawning_service_ready_;
    bool turtle_spawned_;
    rclcpp::Client<turtlesim::srv::Spawn>::SharedPtr spawner_{nullptr};
    rclcpp::TimerBase::SharedPtr timer_{nullptr};
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_{nullptr};
    std::shared_ptr<tf2_ros::TransformListener> tf_listener_{nullptr};
    std::unique_ptr<tf2_ros::Buffer> tf_buffer_;
    std::string target_frame_;
};

int main(int argc, char * argv[])
{
    // ROS2 C++接口初始化
    rclcpp::init(argc, argv);
    // 创建ROS2节点对象并进行初始化,循环等待ROS2退出
    rclcpp::spin(std::make_shared<TurtleFollowing>());
    // 关闭ROS2 C++接口
    rclcpp::shutdown();
    return 0;
}
