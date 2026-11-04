from launch import LaunchDescription
from launch_ros.actions import Node
from moveit_configs_utils import MoveItConfigsBuilder

def generate_launch_description():
    # 加载 Panda 机器人的配置
    moveit_config = MoveItConfigsBuilder("moveit_resources_panda").to_moveit_configs()

    # 配置你的 hello_moveit 节点，并将参数传给它
    hello_moveit_node = Node(
        package="hello_moveit",
        executable="hello_moveit",
        output="screen",
        parameters=[
            moveit_config.robot_description,
            moveit_config.robot_description_semantic,
            moveit_config.robot_description_kinematics,
        ],
    )

    return LaunchDescription([hello_moveit_node])