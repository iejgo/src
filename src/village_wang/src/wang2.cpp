#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/u_int32.hpp"//
//1.导入服务接口(添加依赖（对village_interfaces在xml与Cmake中）)
#include "village_interfaces/srv/sell_novel.hpp"
#include <queue>

using std::placeholders::_1;
using std::placeholders::_2;


class SingleDogNode: public rclcpp::Node//jicheng
{
private:
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_novel;
    /* 声明订阅者、话题发布者 */
    
    rclcpp::Publisher<std_msgs::msg::UInt32>::SharedPtr pub_money;
    //书库
    std::queue<std::string> novels_queue;
     //声明回调组
    rclcpp::CallbackGroup::SharedPtr sell_novels_callback_group;
    //声明服务端
    rclcpp::Service<village_interfaces::srv::SellNovel>::SharedPtr sell_server;

    //话题的回调函数
    void novel_callback(const std_msgs::msg::String::SharedPtr novels)
    {
        std_msgs::msg::UInt32 money;
        money.data = 10;
        pub_money->publish(money);
        //把收到的小说放到书库里
        novels_queue.push(novels->data);
        //编写回调处理逻辑
        RCLCPP_INFO(this->get_logger(),"已阅 %s",novels->data.c_str());

    }
    
    //声明书的价格参数,初始设为一元一章
    unsigned int novel_price = 1;


   
    
    //2.创建买书请求回调函数
    void sell_novel_callback(const village_interfaces::srv::SellNovel::Request::SharedPtr request,
                            const village_interfaces::srv::SellNovel::Response::SharedPtr response)
    {
        //判断当前的书章节数量够不够，不够就要攒书，再返回
        //等待novels_queue 书库里的书来嗯达到我们要卖出去的
        //等待会让当前的线程堵塞
        RCLCPP_INFO(this->get_logger(),"收到一个买书的请求,一共给了%d元",request->money);

        //更新获取价格参数
        this->get_parameter("novel_price",novel_price);
        
        //计算应该返回给客户端的小说数量
        unsigned int num = (int)request->money/novel_price;

        if(num>novels_queue.size())
        {
            //等待，凑齐书
            RCLCPP_INFO(this->get_logger(),"书不够，书库%d,要卖出%d",novels_queue.size(),num);

            rclcpp::Rate rate(1);

            while (novels_queue.size()<num)
            {
                RCLCPP_INFO(this->get_logger(),"等待中，目前还差%d的小说",num-novels_queue.size());
                rate.sleep();
            }
        }
        RCLCPP_INFO(this->get_logger(),"当前书库里的书有%d,大于要卖出的书的数量%d",novels_queue.size(),num);

        for(int i=0;i<(int)num;i++)
        {
            response->novels.push_back(novels_queue.front());
            novels_queue.pop();
        }




    }

public:
   //构造函数
   SingleDogNode(std::string name):Node(name)
{
    RCLCPP_INFO(this->get_logger(),"This is %s, a single man.",name.c_str());
    sub_novel = this->create_subscription<std_msgs::msg::String>("s_girl",10,std::bind(&SingleDogNode::novel_callback,this,_1));
    pub_money = this->create_publisher<std_msgs::msg::UInt32>("s_girl_money",10);
    sell_novels_callback_group = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
    sell_server =this->create_service<village_interfaces::srv::SellNovel>("sell_novel",
                                                                            std::bind(&SingleDogNode::sell_novel_callback,this,_1,_2),
                                                                            rmw_qos_profile_services_default,
                                                                            sell_novels_callback_group);
    //声明参数
    this->declare_parameter<std::int64_t>("novel_price",novel_price);


}
};






int main(int argc,char ** argv)
{
    rclcpp::init(argc,argv);
    auto node = std::make_shared<SingleDogNode>("wang2");

    rclcpp::executors::MultiThreadedExecutor excutor;
    excutor.add_node(node);
    excutor.spin();

    

    
    rclcpp::shutdown();

}