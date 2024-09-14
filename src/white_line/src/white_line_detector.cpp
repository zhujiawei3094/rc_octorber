#include "white_line/white_line.h"

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<WhiteLine>();
  rclcpp::spin(node);
  rclcpp::shutdown();
}
