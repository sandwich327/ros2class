from moveit_configs_utils import MoveItConfigsBuilder
from ament_index_python.packages import get_package_share_directory
from launch_ros.actions import Node
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
import os

def generate_launch_description():
    packagepath = get_package_share_directory('my_robot_moveit_config2')

    moveit_config = (
        MoveItConfigsBuilder("arm", package_name="my_robot_moveit_config2")
        .robot_description('config/my_robot.gazebo11.urdf.xacro')
        .to_moveit_configs()
    )

    # Gazebo
    gazebo_node = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(get_package_share_directory('gazebo_ros'), 'launch', 'gazebo.launch.py')
        ),
        launch_arguments={
            "use_sim_time": "true",
            "verbose": "true",
        }.items()
    )

    # robot_state_publisher（发布 robot_description 和 tf）
    robot_desc_node = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        name="robot_state_publisher",
        output="screen",
        parameters=[
            moveit_config.robot_description,
            {'use_sim_time': True},
            {'publish_frequency': 30.0},
        ],
    )

    # spawn_entity：从 /robot_description 读取 URDF
    robot_to_gazebo_node = Node(
        package="gazebo_ros",
        executable="spawn_entity.py",
        arguments=[
            "-topic", "robot_description",
            "-entity", "my_robot"
        ],
        output="screen",
    )

    return LaunchDescription([
        gazebo_node,
        robot_desc_node,
        robot_to_gazebo_node,
    ])