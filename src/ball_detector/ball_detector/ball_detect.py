#!/usr/bin/env python3
import rclpy
import cv2
import torch

from yolo.yolov5.utils.general import non_max_suppression
from rclpy.node import Node
from cv_bridge import CvBridge

from geometry_msgs.msg import Point
from sensor_msgs.msg import Image


class ballDetect(Node):
    def __init__(self):
        self.declare_parameter("is_detect",True)
        self.declare_parameter("is_red", True)
        self.declare_parameter("is_show", True)

        self.model = torch.hub.load('../yolo/yolov5', 'custom', path='../yolo/model/best.pt', source='local')

        self.subscriber_ = self.create_subscription(Image, "/arm_image", self.arm_image_callback,5)
        self.publisher_ = self.create_publisher(Point, 'ball', 5)
        self.bridge = CvBridge()
        self.point = Point()
        self.subscriber_

    def arm_image_callback(self, msg):
        if self.get_parameter("is_detect").get_paramenter_value().bool_value:
            cv_img = self.bridge.imgmsg_to_cv2(msg, "bgr8")
            self.ball_detect(cv_img)


    def ball_detect(self, cv_img):
        image = cv2.copyMakeBorder(cv_img, 0, 160, 0, 0, cv2.BORDER_CONSTANT, value=[0,0,0])
        resized_img = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        tensor = torch.from_numpy(resized_img).div(255.0).unsqueeze(0)
        tensor = tensor.permute(0,3,1,2)
        re = self.model(tensor.float())
        re = non_max_suppression(re, conf_thres=0.5, iou_thres=0.4)


        if re is not None:
            for pre in re[0]:
                if pre is not None:
                    if self.get_parameter("is_show").get_paramenter_value().bool_value:
                        if pre[4]>pre[5]:
                            self.point.x = (pre[0] + pre[2])/2.0
                            self.point.y = (pre[1] + pre[3])/2.0

                    elif pre[4]<pre[5]:
                        self.point.x = (pre[0] + pre[2])/2.0
                        self.point.y = (pre[1] + pre[3])/2.0
                    self.publisher_.publish(self.point)
                    cv2.rectangle(image, (int(pre[0]), int(pre[1])), (int(pre[2]), int(pre[3])), (0, 0, 255), 2)

        if self.get_parameter("is_show").get_paramenter_value().bool_value:
            cv2.imshow('test',image)
            cv2.waitKey(1)

def main(args=Node):
    rclpy.init(args=args)

    baller = ballDetect()
    rclpy.spin(baller)
    baller.destroy_node()
    rclpy.shutdown()
if __name__ == '__main__':
    main()


