#ifndef RAW_IMAGE
#define RAW_IMAGE

#include "rclcpp/rclcpp.hpp"
#include "opencv2/opencv.hpp"
#include "sensor_msgs/msg/image.hpp"

using namespace std::chrono_literals;

class RawImage : public rclcpp::Node
{
public:
    RawImage();

private:
    using RosImage = sensor_msgs::msg::Image;


    rclcpp::TimerBase::SharedPtr timer;
    cv::VideoCapture arm_capture_, front_capture_;
    rclcpp::Publisher<RosImage>::SharedPtr ros_image_publisher_1;
    rclcpp::Publisher<RosImage>::SharedPtr ros_image_publisher_2;
    RosImage arm_image;
    RosImage front_image;
    cv::Mat font_src,u_font,arm_src,u_arm;

    //font_camera
    cv::Mat font_camera_matrix = (cv::Mat_<float>(3,3)<< 378.862738, 0.000000, 324.008961,
        0.000000, 501.877278, 261.068166,
        0.000000, 0.000000, 1.000000);
    cv::Mat font_distCoeffs = (cv::Mat_<float>(1,5)<< -0.413215, 0.139346, 0.000207, -0.001088, 0.000000);

    void rosimage_callback();
    void arm_image_grab();
    void front_image_grab();


};
#endif
