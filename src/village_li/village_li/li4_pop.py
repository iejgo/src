import rclpy
from rclpy.node import Node


def main(args = None):
    """
    rukouhanshu

    """
    rclpy.init(args = args)
    li4_pop_node = Node("li4_pop")
    li4_pop_node.get_logger().info("This is li4_pop.")
    rclpy.spin(li4_pop_node)
    rclpy.shutdown()
