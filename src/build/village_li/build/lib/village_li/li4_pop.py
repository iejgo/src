import rclpy
from rclpy.node import Node


def main(args = None):
    """
    rukouhanshu

    """
    rclpy.init(args = args)
    li4_node = Node("li4")
    li4_node.get_logger().info("This is li4.")
    rclpy.spin(li4_node)
    rclpy.shutdown()