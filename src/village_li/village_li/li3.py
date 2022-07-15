import rclpy
from rclpy.node import Node 
from std_msgs.msg import String

class BaiPiaoNode(Node):
    def __init__(self,name):#为什么有self??
        super().__init__(name)
        self.get_logger().info("大家好，我是李三，我在白piao李四的小说!")
        self.sub_ = self.create_subscription(String,"s_girl",self.recv_callback,10)
    def recv_callback(self,novel):
        self.get_logger().info('李三：我已经收到了：%s' % novel.data)

def main(args=None):
    rclpy.init(args=args)#初始化rclpy
    node = BaiPiaoNode("li3")#新建一个节点
    rclpy.spin(node) #保持节点运行，检测是否收到退出指令
    rclpy.shutdown()#rcl关闭



