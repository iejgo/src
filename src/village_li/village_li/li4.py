import rclpy
from rclpy.node import Node
from std_msgs.msg import String,UInt32

class WriterNode(Node):
    def __init__(self,name):
        super().__init__(name)
        self.get_logger().info("我是作家李四." % name)
        self.pub_novel = self.create_publisher(String,"s_girl",10)

        self.count = 0 
        self.timer_period = 5
        self.timer = self.create_timer(self.timer_period,self.timer_callback)

        self.account = 80
        self.sub_money = self.create_subscription(UInt32,"s_girl_money",self.recv_money_callback,10)

        #声明参数
        self.declare_parameter("writer_timer_period",5)


    def timer_callback(self):
        """
        定时器回调函数
        """
        #获取并设置参数
        timer_period = self.get_parameter("writer_timer_period").get_parameter_value().integer_value
        self.timer.timer_period_ns = timer_period*(1000*1000*1000)

        msg = String()
        msg.data = "第 %d 回 : 第 %d 次 在清水湖边偶遇" % (self.count,self.count)
        self.pub_novel.publish(msg)
        self.get_logger().info(" 李四新写了一章小说, 这小说的内容是： %s" %msg.data)
        self.count +=1

    def recv_money_callback(self,money):
        self.account += money.data
        self.get_logger().info("获得了 %d 元, 现在总共有 %d 元" %(money.data,self.account))
        

def main(args = None):
    """
    rukouhanshu

    """
    rclpy.init(args = args)
    li4_node = WriterNode("li4")
    rclpy.spin(li4_node)
    rclpy.shutdown()