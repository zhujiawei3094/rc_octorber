#!/usr/bin/env python3
import rclpy
import cv2
from pyzbar.pyzbar import decode

from rclpy.node import Node
from cv_bridge import CvBridge

from geometry_msgs.msg import Point
from sensor_msgs.msg import Image


class QR(Node):
    def __init__(self):
        super().__init__('qr_identify')

        self.declare_parameter('is_qr', True)
        self.declare_parameter('if_qrshow', False)


        self.subscriber_ = self.create_subscription(Image, "/front_image", self.front_image_callback,5)
        self.publisher_ = self.create_publisher(Point, 'tpoic', 5)
        self.bridge = CvBridge()
        self.subscriber_
    def front_image_callback(self, msg):

        cv_img = self.bridge.imgmsg_to_cv2(msg, "bgr8")
        if self.get_parameter('is_qr').get_parameter_value().bool_value:
            self.qr_identify(cv_img)

    def qr_identify(self, image):

#front摄像头的图像使用往往是灰度图，
#后续尝试将消息类型直接改为灰度图

        gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        decoded_objects = decode(gray_image)

        for obj in decoded_objects:
            (x, y, w, h) = obj.rect
            data = obj.data.decode('utf-8')
            if self.get_parameter('if_qrshow').get_parameter_value().bool_value:
                cv2.rectangle(image, (x, y), (x+w, y+h), (0, 0, 0), 4)
                text = "{}".format(data)
                cv2.putText(image, text, (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX,
                                .5, (0, 0, 255), 4)
                cv2.namedWindow("image", 0)
                cv2.resizeWindow("image", 1080, 960)
                cv2.imshow("image", image)
                cv2.waitKey(5)

            pdata = Point()
            pdata.x = x+w/2.0
            pdata.y = y+h/2.0
            pdata.z = float(data)
            self.publisher_.publish(pdata)
            self.get_logger().info(f'Publishing: {pdata.x}, {pdata.y}, {pdata.z}')

def main(args=None):
    rclpy.init(args=args)

    qr_identifier = QR()
    rclpy.spin(qr_identifier)
    qr_identifier.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()

