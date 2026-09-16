/***
@作者: 古月居(www.guyuehome.com)
@说明: ROS2 TF示例-广播动态的坐标变换
***/

#include <functional>
#include <memory>
#include <sstream>
#include <string>


#include "rclcpp/rclcpp.hpp"                       // ROS2 C++接口库
#include "tf2/LinearMath/Quaternion.h"             // 四元数运算库
#include "tf2_ros/transform_broadcaster.h"         // TF坐标变换广播器
#include "turtlesim/msg/pose.hpp"                  // turtlesim小海龟位置消息
#include "geometry_msgs/msg/transform_stamped.hpp" // 坐标变换消息

class TurtleTFBroadcaster : public rclcpp::Node
{
    public:
        TurtleTFBroadcaster()
        : Node("turtle_tf_broadcaster")  // ROS2节点父类初始化
        {
            // 创建一个海龟名称的参数
            turtlename_ = this->declare_parameter<std::string>("turtlename", "turtle");

            // 创建一个TF坐标变换的广播对象并初始化
            tf_broadcaster_ =
                std::make_unique<tf2_ros::TransformBroadcaster>(*this);

            // 使用参数中获取到的海龟名称
            std::ostringstream stream;
            stream << "/" << turtlename_.c_str() << "/pose";
            std::string topic_name = stream.str();

            // 创建一个订阅者，订阅海龟的位置消息
            subscription_ = this->create_subscription<turtlesim::msg::Pose>(
                topic_name, 10,
                std::bind(&TurtleTFBroadcaster::turtle_pose_callback, this, std::placeholders::_1));
        }

    private:
        // 创建一个处理海龟位置消息的回调函数，将位置消息转变成坐标变换
        void turtle_pose_callback(const std::shared_ptr<turtlesim::msg::Pose> msg)
        {
            // 创建一个坐标变换的消息对象
            geometry_msgs::msg::TransformStamped t;

            // 设置坐标变换消息的时间戳
            t.header.stamp = this->get_clock()->now();
            // 设置一个坐标变换的源坐标系
            t.header.frame_id = "world";
            // 设置一个坐标变换的目标坐标系
            t.child_frame_id = turtlename_.c_str();

            // 设置坐标变换中的X、Y、Z向的平移
            t.transform.translation.x = msg->x;
            t.transform.translation.y = msg->y;
            t.transform.translation.z = 0.0;

            // 将欧拉角转换为四元数（roll, pitch, yaw）
            tf2::Quaternion q;
            q.setRPY(0, 0, msg->theta);
            // 设置坐标变换中的X、Y、Z向的旋转（四元数）
            t.transform.rotation.x = q.x();
            t.transform.rotation.y = q.y();
            t.transform.rotation.z = q.z();
            t.transform.rotation.w = q.w();

            // 广播坐标变换，海龟位置变化后，将及时更新坐标变换信息
            tf_broadcaster_->sendTransform(t);
        }

        rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr subscription_;
        std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
        std::string turtlename_;
};

int main(int argc, char * argv[])
{
  // ROS2 Python接口初始化
  rclcpp::init(argc, argv);
  // 创建ROS2节点对象并进行初始化,循环等待ROS2退出
  rclcpp::spin(std::make_shared<TurtleTFBroadcaster>());
  // 关闭ROS2 C++接口
  rclcpp::shutdown();
  return 0;
}
