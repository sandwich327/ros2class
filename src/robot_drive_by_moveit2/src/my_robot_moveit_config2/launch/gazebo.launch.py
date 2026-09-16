from moveit_configs_utils import MoveItConfigsBuilder
from ament_index_python.packages import get_package_share_directory
from launch_ros.actions import Node
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
import os

def generate_launch_description():
    packagepath = get_package_share_directory('my_robot_moveit_config2')
    
    # Load the robot configuration
    moveit_config = (
        MoveItConfigsBuilder("arm", package_name="my_robot_moveit_config2")
        .robot_description('config/my_robot.gazebo.urdf.xacro')
        .robot_description_semantic('config/my_robot.srdf')
        .to_moveit_configs()
    )

    # # 启动 Gazebo：只加载 empty.sdf，使用默认物理引擎（通常是 DART）
    gazebo_node = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            [get_package_share_directory('ros_gz_sim') + '/launch/gz_sim.launch.py']
        ),
        launch_arguments={
            # 去掉 --physics-engine gz-physics-bullet-featherstone-plugin
            'gz_args': 'empty.sdf -r'
        }.items(),
    )
    # gazebo_node = IncludeLaunchDescription(
    #             PythonLaunchDescriptionSource([os.path.join(
    #                 get_package_share_directory('gazebo_ros'), 'launch'), '/gazebo.launch.py']),
    #             launch_arguments={
    #             "use_sim_time" : "true",
    #             "verbose"      : "true",
    #             }.items()
    #          )
    
    # 将机械臂添加到 Gazebo
    # robot_to_gazebo_node = Node(
    #     package='ros_gz_sim',
    #     executable='create',
    #     arguments=[
    #         '-string', moveit_config.robot_description['robot_description'],
    #         '-x', '0.0', '-y', '0.0', '-z', '0.0',
    #         '-name', 'arm'
    #     ]
    # )
    robot_to_gazebo_node = Node(
        package='ros_gz_sim',
        executable='create',
        arguments=[
            '-name', 'arm',
            '-topic', 'robot_description',  # 从 robot_state_publisher 提供的 URDF
        ],
        output='screen',
    )

    # Clock Bridge
    clock_bridge_node = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        arguments=['/clock@rosgraph_msgs/msg/Clock[gz.msgs.Clock]'],
        output='screen'
    )

    # 发布机械臂状态
    robot_desc_node = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        name="robot_state_publisher",
        output="both",
        parameters=[
            moveit_config.robot_description,
            {'use_sim_time': True},   # 使用仿真时间
            {'publish_frequency': 30.0},
        ],
    )

    # Launch RViz
    rviz_node = Node(
        package="rviz2",
        executable="rviz2",
        output="log",
        arguments=["-d", packagepath + '/config/moveit.rviz'],
        parameters=[
            moveit_config.robot_description,
            moveit_config.robot_description_semantic,
            moveit_config.robot_description_kinematics,
            moveit_config.planning_pipelines,
            moveit_config.joint_limits,
            {'use_sim_time': True},
        ],
    )

    # ros2_control manager 节点
    ros2_control_node = Node(
        package="controller_manager",
        executable="ros2_control_node",
        parameters=[
            packagepath + '/config/ros2_controllers.yaml',
            {'use_sim_time': True}
        ],
        output="both",
    )

    # 自动关节状态发布器，arm 控制器，夹抓控制器
    controller_spawner_node = Node(
        package="controller_manager",
        executable="spawner",
        arguments=[
            "joint_state_broadcaster",
            "arm_controller",
            "gripper_controller"
        ],
    )

    # 启动 move_group node/action server
    move_group_node = Node(
        package="moveit_ros_move_group",
        executable="move_group",
        output="screen",
        parameters=[
            moveit_config.to_dict(),
            {'use_sim_time': True}
        ],
    )

    return LaunchDescription([
        gazebo_node,
        robot_to_gazebo_node,
        clock_bridge_node,
        robot_desc_node,
        rviz_node,
        ros2_control_node,
        controller_spawner_node,
        move_group_node
    ])