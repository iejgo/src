import rclpy
from rclpy.node import Node
from std_msgs.msg import String,UInt32

class WriterNode(Node):
    def __init__(self,name):
        super().__init__(name)
        self.get_logger().info("This is %s, a writer." % name)
        self.pub_novel = self.create_publisher(String,"s_girl",10)

        self.count = 0 
        self.timer_period = 5
        self.timer = self.create_timer(self.timer_period,self.timer_callback)

        self.account = 80
        self.sub_money = self.create_subscription(UInt32,"s_girl_money",self.recv_money_callback,10)



    def timer_callback(self):
        msg = String()
        msg.data = "The %d times : Run into s_girl for %d times by the lake" % (self.count,self.count)
        self.pub_novel.publish(msg)
        self.get_logger().info("Li4 publishes a chapter of the novel, and the content is %s" %msg.data)
        self.count +=1

    def recv_money_callback(self,money):
        self.account += money.data
        self.get_logger().info("Get %d money, there are %d money" %(money.data,self.account))
        

def main(args = None):
    """
    rukouhanshu

    """
    rclpy.init(args = args)
    li4_node = WriterNode("li4")
    rclpy.spin(li4_node)
    rclpy.shutdown()