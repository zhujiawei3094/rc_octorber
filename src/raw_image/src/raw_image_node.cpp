#include "raw_image/raw_image.h"
#include "rclcpp/rclcpp.hpp"

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<RawImage>();
    rclcpp::spin(node);
    rclcpp::shutdown();
}
