from launch_ros.actions import Node
from launch import LaunchDescription
from launch.substitutions import PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare

def generate_launch_description():
    variedInterval_cmd = Node(
        package='param_pkg',
        executable='variedInterval',
        name='variedInterval_node',
        parameters=[{"tmrInterval": 10.0}]
        )
       
    ld = LaunchDescription()
    ld.add_action(variedInterval_cmd)
    return ld
