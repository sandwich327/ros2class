/***
@作者: 古月居(www.guyuehome.com)
@说明: ROS2 TF示例-广播静态的坐标变换
***/

#include "rclcpp/rclcpp.hpp"                         // ROS2 C++接口库
#include "tf2/LinearMath/Quaternion.h"               // 四元数计算库
#include "tf2_ros/static_transform_broadcaster.h"    // TF静态坐标系广播器类
#include "geometry_msgs/msg/transform_stamped.hpp"   // 坐标变换消息

class StaticTFBroadcaster : public rclcpp::Node
{
    public:
        explicit StaticTFBroadcaster()
        : Node("static_tf_broadcaster")      // ROS2节点父类初始化
        {
            // 创建一个TF广播器对象
            tf_static_broadcaster_ = std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);

            // 广播静态坐标变换，广播后两个坐标系的位置关系保持不变
            this->make_transforms();
        }

    private:
        void make_transforms()
        {
            // 创建一个坐标变换的消息对象
            geometry_msgs::msg::TransformStamped t; 

            // 设置坐标变换消息的时间戳
            t.header.stamp = this->get_clock()->now();
            // 设置一个坐标变换的源坐标系
            t.header.frame_id = "world";
            // 设置一个坐标变换的目标坐标系
            t.child_frame_id = "house";

            // 设置坐标变换中的X、Y、Z向的平移
            t.transform.translation.x = 10.0;
            t.transform.translation.y = 5.0;
            t.transform.translation.z = 0.0;

            // 将欧拉角转换为四元数（roll, pitch, yaw）
            tf2::Quaternion q;
            q.setRPY(0.0, 0.0, 0.0);
            
            // 设置坐标变换中的X、Y、Z向的旋转（四元数）
            t.transform.rotation.x = q.x();
            t.transform.rotation.y = q.y();
            t.transform.rotation.z = q.z();
            t.transform.rotation.w = q.w();

            // 广播静态坐标变换
            tf_static_broadcaster_->sendTransform(t);
        }

        std::shared_ptr<tf2_ros::StaticTransformBroadcaster> tf_static_broadcaster_;
};

int main(int argc, char * argv[])
{
    // ROS2 C++接口初始化
    rclcpp::init(argc, argv);
    // 创建ROS2节点对象并进行初始化,循环等待ROS2退出
    rclcpp::spin(std::make_shared<StaticTFBroadcaster>());
    // 关闭ROS2 C++接口
    rclcpp::shutdown();

    return 0;
}
