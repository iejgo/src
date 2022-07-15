#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/u_int32.hpp"//
//1.导入服务接口(添加依赖（对village_interfaces在xml与Cmake中）)
#include "village_interfaces/srv/sell_novel.hpp"
#include <queue>

using std::placeholders::_1;


class SingleDogNode: public rclcpp::Node//jicheng
{
private:
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_novel;
    /* data */
    
    rclcpp::Publisher<std_msgs::msg::UInt32>::SharedPtr pub_money;

    void novel_callback(const std_msgs::msg::String::SharedPtr novels)
    {
        std_msgs::msg::UInt32 money;
        money.data = 10;
        pub_money->publish(money);

        RCLCPP_INFO(this->get_logger(),"See %s",novels->data.c_str());

    }
    //书库
    std::queue<std::string> novels_queue;

    //声明回调组
    rclcpp novel_callback(const std_msgs::msg::String::SharedPtr novels)



    //2.创建买书请求回调函数
    void sell_novel_callback(const village_interfaces::srv::SellNovel::Request::SharedPtr request,
                            const village_interfaces::srv::SellNovel::Response::SharedPtr response)
    {
        //判断当前的书章节数量够不够，不够就要攒书，再返回
        //等待novels_queue 书库里的书来嗯达到我们要卖出去的
        //等待会让当前的线程堵塞

    }

public:
  
   SingleDogNode(std::string name):Node(name)
{
    RCLCPP_INFO(this->get_logger(),"This is %s, a single man.",name.c_str());
    sub_novel = this->create_subscription<std_msgs::msg::String>("s_girl",10,std::bind(&SingleDogNode::novel_callback,this,_1));
    pub_money = this->create_publisher<std_msgs::msg::UInt32>("s_girl_money",10);

}
};






int main(int argc,char ** argv)
{
    rclcpp::init(argc,argv);
    auto node = std::make_shared<SingleDogNode>("wang2");

    rclcpp::executors::MultiThreadedExecutor excutor;
    excutor.add_node(node);
    excutor.spin();

    

    rclcpp::spin(node);
    rclcpp::shutdown();

}