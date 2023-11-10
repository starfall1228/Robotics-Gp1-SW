#!/usr/bin/env python3
# -*- coding:utf-8 -*-
import cv2
import numpy as np
from tkinter import Tk
from PIL import Image
# You should have no reason to modify this class
import path_signals as ps 
class ColorDisplayWindow:
    def __init__(
        self,
        group_name="Group name here",
        window_name="displayWindow1",
        window_height=Tk().winfo_screenheight(),
        window_width=Tk().winfo_screenwidth(),
    ):
        self.group_name = group_name
        self.window_name = window_name

        self.title_height = 100
        self.window_height = window_height - self.title_height
        self.window_width = window_width

        self.window = np.zeros((window_height, window_width, 3), dtype=np.uint8)

        self.color_detection_count = 4
        self.color_box_width = int(self.window_width / self.color_detection_count)
        self.color_box_height = self.window_height

        self.font = cv2.FONT_HERSHEY_SIMPLEX
        self.font_scale = 2
        self.font_thickness = 5

    # helper function to draw the color boxes
    def display(self, __color_detection_list):
        # add a white title bar
        self.window[self.window_height :, :] = (255, 255, 255)

        # add the title "RDC 2023 Color Detection"
        title = "RDC 2023 Color Detection : " + self.group_name
        title_size = cv2.getTextSize(
            title, self.font, self.font_scale, self.font_thickness
        )[0]
        title_x = (self.window_width - title_size[0]) // 2
        title_y = self.window_height + ((self.title_height + title_size[1]) // 2)
        cv2.putText(
            self.window,
            title,
            (title_x, title_y),
            self.font,
            self.font_scale,
            (0, 0, 0),
            self.font_thickness,
            cv2.LINE_AA,
        )

        for i in range(self.color_detection_count):
            # draw the color box
            color = __color_detection_list[i]
            color_box = np.zeros(
                (self.color_box_height, self.color_box_width, 3), dtype=np.uint8
            )
            color_box[:] = color
            self.window[
                : self.color_box_height,
                i * self.color_box_width : (i + 1) * self.color_box_width,
            ] = color_box

            # prepare the number at the center of each color box
            number = str(i + 1)
            text_size = cv2.getTextSize(
                number, self.font, self.font_scale, self.font_thickness
            )[0]
            text_x = (i * self.color_box_width) + (
                (self.color_box_width - text_size[0]) // 2
            )
            text_y = (self.color_box_height + text_size[1]) // 2

            # Draw a black square beneath the number
            square_size = text_size[1] + 40  # Add some padding
            square_top_left = (
                text_x - int(0.5 * (square_size // 2)),
                text_y - 3 * (text_size[1] // 2),
            )
            square_bottom_right = (
                text_x + int(1.5 * (square_size // 2)),
                text_y + (text_size[1] // 2),
            )
            cv2.rectangle(
                self.window,
                square_top_left,
                square_bottom_right,
                (0, 0, 0),
                -1,
                cv2.LINE_AA,
            )

            # Draw the number
            cv2.putText(
                self.window,
                number,
                (text_x, text_y),
                self.font,
                self.font_scale,
                (255, 255, 255),
                self.font_thickness,
                cv2.LINE_AA,
            )

        cv2.namedWindow(self.window_name, cv2.WND_PROP_FULLSCREEN)
        cv2.setWindowProperty(
            self.window_name, cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN
        )
        cv2.imshow(self.window_name, self.window)




def get_limits(color):
    c = np.uint8([[color]])  # BGR values
    hsvC = cv2.cvtColor(c, cv2.COLOR_BGR2HSV)

    hue = hsvC[0][0][0]  # Get the hue value

    # Handle red hue wrap-around
    if hue >= 165:  # Upper limit for divided red hue
        lowerLimit = np.array([hue - 10, 100, 100], dtype=np.uint8)
        upperLimit = np.array([180, 255, 255], dtype=np.uint8)
    elif hue <= 15:  # Lower limit for divided red hue
        lowerLimit = np.array([0, 100, 100], dtype=np.uint8)
        upperLimit = np.array([hue + 10, 255, 255], dtype=np.uint8)
    else:
        lowerLimit = np.array([hue - 10, 100, 100], dtype=np.uint8)
        upperLimit = np.array([hue + 10, 255, 255], dtype=np.uint8)

    return lowerLimit, upperLimit

lower_green,upper_green = get_limits([0,255,0])
lower_blue,upper_blue = get_limits([255,0,0])

lower_red,upper_red = get_limits([0,0,255])

# order BGR
yellow = [0,255,255]
green = [0,255,0]
blue = [255,0,0]
red = [0,0,255]




cap = cv2.VideoCapture(0)
if __name__ == "__main__":
    # TODO: Change your team's name
    color_display_1 = ColorDisplayWindow(
        group_name="Isabella 1",
    )

    color_detection_list = [[0, 0, 0], [0, 0, 0], [0, 0, 0], [0, 0, 0]]
    count = 0
    while True:
         
        ret, frame = cap.read()
        # hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        # TODO: You should constantly update the values of color_detection_list based on the BGR values you get
        # color_detection_list[0][0] = (color_detection_list[0][0] + 1) % 255
        # color_detection_list[1][1] = (color_detection_list[1][1] + 1) % 255
        # color_detection_list[2][2] = (color_detection_list[2][2] + 1) % 255
        # color_detection_list[3] = [
        #     (color_detection_list[3][0] + 1) % 255,
        #     0,
        #     (color_detection_list[3][2] + 1) % 255,
        # ]

        # mask_blue = cv2.inRange(hsv_frame, lower_blue, upper_blue)
        # mask_red = cv2.inRange(hsv_frame, lower_red, upper_red)
        

        hsvImage = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        # cropping the image 
        
        # Cropping image 1 
        lowerLimitGreen, upperLimitGreen = get_limits(color=green)
        maskGreen = cv2.inRange(hsvImage, lowerLimitGreen, upperLimitGreen)
        maskGreen_ = Image.fromarray(maskGreen)

        
        # Detecting blue color 
        lowerLimitBlue, upperLimitBlue = get_limits(color=blue)
        maskBlue = cv2.inRange(hsvImage, lowerLimitBlue, upperLimitBlue)
        maskBlue_ = Image.fromarray(maskBlue)
        
        
        # Detecting red color
        bbox = maskGreen_.getbbox()
        lowerLimitRed, upperLimitRed = get_limits(color=red)
        maskRed = cv2.inRange(hsvImage, lowerLimitRed, upperLimitRed)
        maskRed_ = Image.fromarray(maskRed)

        if bbox is not None:
            x1, y1, x2, y2 = bbox
            frame = cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 5)

          
        if np.any(maskGreen_):
            print("Green")

        # if np.any(maskRed):
        #     print("Green")

        # if np.any(maskBlue_):
        #     print("Blue")
        

        cv2.imshow('frame', frame) 
        # cv2.imshow('hsv',hsvImage)
        # Updates display
        # color_display_1.display(color_detection_list)
        # print(frame) 
        if cv2.waitKey(10) & 0xFF == ord("q"):  # waits for 'q' key to be pressed
            # Sending signal to the robot so that the robot can be terminated
            # For shutting down the robot when necessary
            break

    cv2.destroyAllWindows()
