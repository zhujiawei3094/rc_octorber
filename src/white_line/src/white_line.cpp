//
// Created by zhu on 24-8-6.
//
#include "../include/white_line/white_line.h"

WhiteLine::WhiteLine():Node("white_line")
{
    front_image = this->create_subscription<RosImage>("/front_image",5,std::bind(&WhiteLine::front_image_callback,this,std::placeholders::_1));
}


void WhiteLine::front_image_callback(const RosImage &msg)
{
    cv_bridge::CvImagePtr cv_image =cv_bridge::toCvCopy(msg,"bgr8");

    cv::Canny(cv_image->image,edge,50,100,3);
    cv::cvtColor(edge, gray, CV_GRAY2BGR );
    find_line();
    find_line_P();
}

void WhiteLine::find_line()
{
    cv::HoughLines(edge,lines,1,CV_PI/180.0,150,0,0);

    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];
        cv::Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        cv::line( gray, pt1, pt2, cv::Scalar(0,0,255), 3, cv::LINE_AA);
    }
    cv::imshow("1",gray);
    cv::waitKey(1);

}

void WhiteLine::find_line_P()
{
    cv::HoughLinesP(edge, linesP,1,CV_PI/180.0,50,50,10);

    for( size_t i = 0; i < linesP.size(); i++ )
    {
        cv::Vec4i l = linesP[i];
        cv::line( gray, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255), 3, cv::LINE_AA);
    }
    cv::imshow("p",gray);
    cv::waitKey(1);
}

