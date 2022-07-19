#1.导入头文件
from tkinter.font import names
from launch import LaunchDescription
from launch_ros.actions import Node


#2.定义函数
def generate_launch_description():
    #创建节点描述
    li4_node = Node(
        package = "village_li",
        executable="li4_node",
        parameters=[{'writer_timer_period':1}],
       #namespace="mirror"

    )
    wang2_node = Node(
        package = "village_wang",
        executable="wang2_node"
    )
#4.launch_description
    launch_d = LaunchDescription([li4_node,wang2_node])
    return launch_d





