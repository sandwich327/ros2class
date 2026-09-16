from launch_ros.actions import Node
from launch import LaunchDescription
from launch.substitutions import PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare

def generate_launch_description():
    publisher_cmd = Node(
        package='param_pkg',
        executable='param_server',
        name='minimal_param_server',
        parameters=[{"launchparam": "5"},
                    # 方式2：从YAML文件加载
                PathJoinSubstitution([
                    FindPackageShare('param_pkg'),
                    'config',
                    'params.yaml'
                ])]
        )
    
    subscriber_cmd = Node(
        package='param_pkg',
        executable='param_client',
        name='paramClient_node'
        )
    
    ld = LaunchDescription()
    ld.add_action(publisher_cmd)
    ld.add_action(subscriber_cmd)
    return ld
