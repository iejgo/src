#include "rclcpp/rclcpp.hpp"

int main(int argc,char ** argv)
{
    rclcpp::init(argc,argv);
    auto node = std::make_shared<rclcpp::Node>("wang2");
    
    RCLCPP_INFO(node->get_logger(),"This is wang2, a single man.");

    rclcpp::spin(node);
    rclcpp::shutdown();

}