from launch_ros.actions import Node
from launch import LaunchDescription

def generate_launch_description():
    server_cmd = Node(
        package='srv_pkg',
        executable='myserver',
        name='myserver_node'
        )
    client_cmd = Node(
        package='srv_pkg',
        executable='myclient',
        name='myclient_node',
        arguments=['1', '2'] 
        )
    ld = LaunchDescription()
    ld.add_action(server_cmd)
    ld.add_action(client_cmd)
    return ld
