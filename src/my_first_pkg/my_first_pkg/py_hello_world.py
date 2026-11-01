import rclpy
from rclpy.node import Node
from rclpy.timer import Timer

class HelloWorldNode(Node):
    def __init__(self):
        super().__init__('hello_world_node')
        
        self.timer = self.create_timer(0.5, self.timer_callback)
        
        self.get_logger().info('节点已启动，开始循环打印 Hello World...')

    def timer_callback(self):
        self.get_logger().info('Hello World')

def main(args=None):
    rclpy.init(args=args)
    
    node = HelloWorldNode()
    
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()
