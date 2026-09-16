/***
@作者: 古月居(www.guyuehome.com)
@说明: ROS2 TF示例-监听某两个坐标系之间的变换
***/

#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "tf2/exceptions.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"

using namespace std::chrono_literals;

class TFListener : public rclcpp::Node
{
    public:
        TFListener()
        : Node("tf_listener")  //ROS2节点父类初始化
        {
            // 创建一个目标坐标系名的参数,优先使用外部设置的参数值，否则用默认值
            target_frame_ = this->declare_parameter<std::string>("target_frame", "house");
            
            // 创建保存坐标变换信息的缓冲区
            tf_buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());
            
            // 创建坐标变换的监听器
            tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);

            // 创建一个固定周期的定时器，处理坐标信息
            timer_ = this->create_wall_timer(1s, std::bind(&TFListener::on_timer, this));
        }

    private:
        void on_timer()
        {
            // 设置源坐标系和目标坐标系的名称
            std::string target_frame = target_frame_.c_str();
            std::string source_frame = "world";

            geometry_msgs::msg::TransformStamped trans;

            // 监听当前时刻源坐标系到目标坐标系的坐标变换
            try {
                trans = tf_buffer_->lookupTransform(
                            target_frame, source_frame, tf2::TimePointZero);
            } catch (const tf2::TransformException & ex) {
                // 如果坐标变换获取失败，进入异常报告
                RCLCPP_INFO(
                    this->get_logger(), "Could not transform %s to %s: %s",
                    target_frame.c_str(), source_frame.c_str(), ex.what());
                return;
            }
            
            // 四元数转换为欧拉角
            tf2::Quaternion q(
                trans.transform.rotation.x,
                trans.transform.rotation.y,
                trans.transform.rotation.z,
                trans.transform.rotation.w);
            tf2::Matrix3x3 m(q);
            double roll, pitch, yaw;
            m.getRPY(roll, pitch, yaw);
            
            
            // 打印查询到的坐标信息
            RCLCPP_INFO(
                this->get_logger(), "Get %s --> %s transform: [%f, %f, %f] [%f, %f, %f]",
                source_frame.c_str(), target_frame.c_str(), 
                trans.transform.translation.x, trans.transform.translation.y, trans.transform.translation.z,
                roll, pitch, yaw);

        }

        rclcpp::TimerBase::SharedPtr timer_{nullptr};
        std::shared_ptr<tf2_ros::TransformListener> tf_listener_{nullptr};
        std::unique_ptr<tf2_ros::Buffer> tf_buffer_;
        std::string target_frame_;
};

int main(int argc, char * argv[])
{
    // ROS2 C++接口初始化
    rclcpp::init(argc, argv);
    // 创建ROS2节点对象并进行初始化,循环等待ROS2退出
    rclcpp::spin(std::make_shared<TFListener>());
    // 关闭ROS2 C++接口
    rclcpp::shutdown();
    return 0;
}
