#!/usr/bin/env python3
import rclpy
import cv2
from pyzbar.pyzbar import decode

from rclpy.node import Node

from geometry_msgs.msg import Point

class QR(Node):
    def __init__(self):
        super().__init__('qr_identify')

        self.declare_parameter('is_qr', True)
        self.declare_parameter('if_qrshow', False)

        self.cap = cv2.VideoCapture(2)

        if not self.cap.isOpened():
            print("can't open camera")
            exit()

        self.publisher_ = self.create_publisher(Point, 'tpoic', 10)
        self.timer = self.create_timer(0.5, self.timer_callback)

    def timer_callback(self):
        if self.get_parameter('is_qr').get_parameter_value().bool_value:
            self.qr_identify()

    def qr_identify(self):
        ret, image = self.cap.read()

        if not ret:
            print("can't get image")

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

