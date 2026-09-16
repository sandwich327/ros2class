from ament_index_python.packages import get_package_share_path
from launch import launch_description_sources
from launch import actions
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition, UnlessCondition
from launch.substitutions import Command, LaunchConfiguration
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue
import os

def generate_launch_description():
    urdf_tutorial_path = get_package_share_path('mbot_description')#find share directory in mbot_description
    # 组装字符串路径
    default_model_path = os.path.join(urdf_tutorial_path, 'urdf', 'mbot_withinertial.xacro')
    default_rviz_config_path = os.path.join(urdf_tutorial_path, 'config', 'display_mbotbase.rviz')
    default_gazebo_world_path = os.path.join(urdf_tutorial_path, 'world', 'my_room.world')

    # 声明可配置参数
    model_arg = DeclareLaunchArgument(
        name='model',
        default_value=default_model_path,
        description='Absolute path to robot xacro file'
    )
    
    rviz_arg = DeclareLaunchArgument(
        name='rvizconfig',
        default_value=default_rviz_config_path,
        description='Absolute path to rviz config file'
    )

    # 用 xacro 生成 robot_description（Command 的每个元素必须是 str 或 Substitution）
    substitutions_command = Command(['xacro ', LaunchConfiguration('model')])
    robot_description = ParameterValue(substitutions_command, value_type=str)

    robot_state_publisher_node = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[{'robot_description': robot_description}, {'use_sim_time': True}] #use_sim_time to avoid warning
    )

    joint_state_publisher_node = Node(
        package='joint_state_publisher',
        executable='joint_state_publisher',
        parameters=[{'use_sim_time': True}]
    )

    rviz_node = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        output='screen',
        arguments=['-d', LaunchConfiguration('rvizconfig')]
    )

    # 引用 Gazebo 的 launch（传入字符串路径）
    gazebo_pkg_share = get_package_share_path('gazebo_ros')
    gazebo_launch_path = os.path.join(gazebo_pkg_share, 'launch', 'gazebo.launch.py')

    #show the world
    action_launch_gazebo = actions.IncludeLaunchDescription(
        launch_description_sources.PythonLaunchDescriptionSource(gazebo_launch_path),
        launch_arguments={
            'world': default_gazebo_world_path,
            'verbose': 'true'
        }.items()
    )
    
    #show the robot, the topic is the same name as robot_state_publisher_node
    action_spawn_entity = Node(
        package='gazebo_ros',
        executable='spawn_entity.py',
        arguments=['-topic','/robot_description','-entity','mbot'],
        output='screen'
    )
    
    return LaunchDescription([
        model_arg,
        rviz_arg,
        joint_state_publisher_node,
        robot_state_publisher_node,
        rviz_node,
        action_launch_gazebo,
        action_spawn_entity
    ])
    