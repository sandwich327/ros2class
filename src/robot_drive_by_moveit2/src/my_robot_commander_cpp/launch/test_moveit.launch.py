from launch import LaunchDescription
from launch_ros.actions import Node
from moveit_configs_utils import MoveItConfigsBuilder

def generate_launch_description():
    # 注意：这里的 "my_robot" 需要替换为你实际的 moveit_config 包名对应的前缀
    # 例如，如果你的配置包叫 my_robot_moveit_config，这里就填 "my_robot"
    moveit_config = MoveItConfigsBuilder("my_robot").to_moveit_configs()

    test_moveit_node = Node(
        package="my_robot_commander_cpp",
        executable="test_moveit",
        output="screen",
        parameters=[
            moveit_config.robot_description,
            moveit_config.robot_description_semantic,
            moveit_config.robot_description_kinematics, # 传入运动学参数，消除警告
        ],
    )

    return LaunchDescription([test_moveit_node])
