//
// Created by zhu on 24-8-6.
//

#ifndef WHITE_LINE_H
#define WHITE_LINE_H

#include "rclcpp/rclcpp.hpp"
#include "opencv2/opencv.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "geometry_msgs/msg/point.hpp"
#include "cv_bridge/cv_bridge.h"

class WhiteLine : public rclcpp::Node
{
public:
    WhiteLine();

private:
    using RosImage = sensor_msgs::msg::Image;
    using Point = geometry_msgs::msg::Point;

    rclcpp::Subscription<RosImage>::SharedPtr front_image;

    cv::Mat gray,edge;

    std::vector<cv::Vec2f> lines;
    std::vector<cv::Vec4i> linesP;

    void front_image_callback(const RosImage &msg);
    void find_line();
    void find_line_P();
};

#endif //WHITE_LINE_H
