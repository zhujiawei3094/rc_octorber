//
// Created by zhu on 24-9-9.
//

#ifndef GET_DATA_H
#define GET_DATA_H

#include "opencv2/opencv.hpp"

class GetData
{
public:
   GetData()
   {
      cap.open("/dev/video2");
   }
   void get_data(int key)
   {
      char * head = "../Data/",* end = ".jpg";
      char * path = new char[50];
      sprintf(path,"%s%d%s",head,i,end);
      cap.read(raw);
      cv::undistort(raw,src,font_camera_matrix,font_distCoeffs);
      cv::imshow("data",src);

      if(key==1)
      {
         cv::imwrite(path,src);
         i++;
         std::cout << i << std::endl;
      }
      cv::waitKey(1);

   }
   int i=0;

private:
   cv::VideoCapture cap;
   cv::Mat raw,src;

   cv::Mat font_camera_matrix = (cv::Mat_<float>(3,3)<< 378.862738, 0.000000, 324.008961,
        0.000000, 501.877278, 261.068166,
        0.000000, 0.000000, 1.000000);
   cv::Mat font_distCoeffs = (cv::Mat_<float>(1,5)<< -0.413215, 0.139346, 0.000207, -0.001088, 0.000000);


};
#endif //GET_DATA_H
