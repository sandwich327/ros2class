from launch import LaunchDescription
from launch_ros.actions import Node
from moveit_configs_utils import MoveItConfigsBuilder
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    package_name = "my_robot_moveit_config2"
    share_dir = get_package_share_directory(package_name)

    # MoveItConfigsBuilder 这个工具会帮你自动生成一堆 MoveIt 需要的参数，最核心的就是：
    # robot_description：
    # 通常是一个字符串，里面是你的 URDF（由 xacro 展开后的结果）
    # robot_description_semantic：
    # SRDF 内容（规划组、关节限制、碰撞禁用等）
    # 以及：
    # robot_description_kinematics
    # planning_pipelines
    # moveit_controller_manager
    # 等等 MoveIt 相关的配置
    # robot_description("config/my_robot.gazebo11.urdf.xacro")
    # 这行的意思是：用这个 xacro 文件生成 robot_description 参数。
    # to_moveit_configs() 会把这些配置打包成一个方便使用的对象，内部已经准备好了各种参数的字典结构。
    moveit_config = (
        MoveItConfigsBuilder("arm", package_name=package_name)
        .robot_description("config/my_robot.gazebo11.urdf.xacro")
        .robot_description_semantic("config/my_robot.srdf")
        .trajectory_execution("config/moveit_controllersg11.yaml")
        .to_moveit_configs()
    )

    test_moveit_node = Node(
        package="my_robot_commander_cpp",      
        executable="jointPlanning",            
        name="test_moveit",
        output="screen",
        parameters=[
            moveit_config.to_dict(), #moveit_config.to_dict() 返回一个 Python 字典 make them as param list
            {"use_sim_time": True},
        ],
    )

    return LaunchDescription([
        test_moveit_node,
    ])