#include "rclcpp/rclcpp.hpp"
#include "village_interfaces/srv/sell_novel.hpp"//通过修改CMake与package.xml

//提前声明的占位符，留着创建客户端的时候用
using std::placeholders::_1;

/* 创建一个类节点，名字叫做PoolManNode,继承自Node */
class PoorManNode : public rclcpp::Node
{
public:
    /*构造函数*/
    PoorManNode(std::string name) : Node(name)
    {
        //打印一句自我介绍
        RCLCPP_INFO(this->get_logger(),"大家好，我是poorman张三.");
        //客户端创建
        novel_client = this->create_client<village_interfaces::srv::SellNovel>("sell_novel");
        
    }
    /*编写发送请求逻辑*/
    void buy_novels()
    {
        RCLCPP_INFO(this->get_logger(),"准备去买小说");
        //等待服务端上线
        while(!novel_client->wait_for_service(std::chrono::seconds(1)))
        {
            RCLCPP_WARN(this->get_logger(),"等待服务端上线");
        }
        //构造请求数据
        auto request = std::make_shared<village_interfaces::srv::SellNovel_Request>();
        request->money = 5;
        //发送异步请求
        novel_client->async_send_request(request,std::bind(&PoorManNode::novels_callback,this,_1));
    }

private:
    //声明客户端
    rclcpp::Client<village_interfaces::srv::SellNovel>::SharedPtr novel_client;

    //接收来自服务端的返回结果的回调函数
    void novels_callback(rclcpp::Client<village_interfaces::srv::SellNovel>::SharedFuture response)
    {
        auto result = response.get();

        RCLCPP_INFO(this->get_logger(),"收到了%d章的小说，现在开始按章节读",result->novels.size());//novels哪来的？

        for(std::string novel:result->novels) //??
        {
            //打印每一章小说内容
            RCLCPP_INFO(this->get_logger(),"%s",novel.c_str());//novel哪里来的？
        }

        RCLCPP_INFO(this->get_logger(),"小说看完了，写的真不错，价格实惠");

    }

};

/*主函数*/
int main(int argc, char **argv)
{
    rclcpp::init(argc,argv);
    /*产生一个zhang3的节点*/
    auto node =  std::make_shared<PoorManNode>("zhang3");
    node->buy_novels();
    /*运行节点，并检测rclcpp状态*/
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

