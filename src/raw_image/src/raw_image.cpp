#include "raw_image/raw_image.h"
using std::placeholders::_1;
RawImage::RawImage():Node("raw_image")
{
    declare_parameter("fps",30);

    arm_capture_.open(2);
    //front_capture_.open(2);

    ros_image_publisher_1 = this->create_publisher<RosImage>("/arm_image",5);
    ros_image_publisher_2 = this->create_publisher<RosImage>("/front_image", 5);
    timer = this->create_wall_timer(1s/get_parameter("fps").as_int(), std::bind(&RawImage::rosimage_callback,this) );

}

void RawImage::rosimage_callback()
{
    arm_image_grab();
    front_image_grab();
}
void RawImage::arm_image_grab()
{
    arm_capture_.read(arm_src);
    if(arm_src.empty())
    {
        RCLCPP_ERROR(this->get_logger(),"arm can't get image");
        return;
    }
//消畸变
    cv::undistort(arm_src,u_arm,font_camera_matrix,font_distCoeffs);
    auto stamp = now();
    arm_image.header.stamp = stamp;
    arm_image.header.frame_id = "arm_image";
    arm_image.height = u_arm.rows;
    arm_image.width = u_arm.cols;
    arm_image.encoding = "bgr8";
    arm_image.is_bigendian = false;
    arm_image.step = static_cast<RosImage::_step_type>(u_arm.step);
    arm_image.data.assign(u_arm.datastart,u_arm.dataend);

    ros_image_publisher_1->publish(arm_image);
}

void RawImage::front_image_grab()
{
    front_capture_.read(font_src);
    if(font_src.empty())
    {
        RCLCPP_ERROR(this->get_logger(),"front can't get image");
        return;
    }
    cv::undistort(font_src,u_font,font_camera_matrix,font_distCoeffs);
    auto stamp = now();
    front_image.header.stamp = stamp;
    front_image.header.frame_id = "front_image";
    front_image.height = u_font.rows;
    front_image.width = u_font.cols;
    front_image.encoding = "bgr8";
    front_image.is_bigendian = false;
    front_image.step = static_cast<RosImage::_step_type>(u_font.step);
    front_image.data.assign(u_font.datastart,u_font.dataend);
    
    ros_image_publisher_2->publish(front_image);
}





