#!/usr/bin/env python3
# -*- coding:utf-8 -*-
import cv2
import numpy as np
from tkinter import Tk


# You should have no reason to modify this class
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


if __name__ == "__main__":
    # TODO: Change your team's name
    color_display_1 = ColorDisplayWindow(
        group_name="RDC Team 1",
    )
    color_detection_list = [[0, 0, 0], [0, 0, 0], [0, 0, 0], [0, 0, 0]]
    boundaries = [
        (lower_red, upper_red),
        (lower_green, upper_green),
        (lower_blue, upper_blue)
        ]
    capture = cv2.VideoCapture(0)
    while True:
        # TODO: You should constantly update the values of color_detection_list based on the BGR values you get
        # color_detection_list[0][0] = (color_detection_list[0][0] + 1) % 255
        # color_detection_list[1][1] = (color_detection_list[1][1] + 1) % 255
        # color_detection_list[2][2] = (color_detection_list[2][2] + 1) % 255
        # color_detection_list[3] = [
        #     (color_detection_list[3][0] + 1) % 255,
        #     0,
        #     (color_detection_list[3][2] + 1) % 255,
        # ]
        ret, frame = capture.read()
        height = frame.shape[0]
        width = frame.shape[1]
        blurredGaus = cv2.GaussianBlur(frame, (5, 5), 0)    
        blurredMedian = cv2.medianBlur(frame, 5)
        img = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        # img = frame
        # img = cv2.GaussianBlur(img, (11,11), 0)
        # img = frame
        cury = 0
        part = int(width / 4)
        colors = ["","","",""]
        imgs = []
        for i in range(4) :        
            ind = 0
            nexy = min(cury + part,width)
            cropped = img[0:height - 1, cury:nexy - 1]
            cropped1 = frame[0:height - 1, cury:nexy - 1]
            for (lower, upper) in boundaries:
                # print(lower, upper)
                lower = np.array(lower, dtype = "uint8")
                upper = np.array(upper, dtype = "uint8")
                mask = cv2.inRange(cropped, lower, upper)
                output = cv2.bitwise_and(cropped, cropped, mask = mask)
                color_detection_list[i][ind] = np.count_nonzero(output)
                ind += 1
            if color_detection_list[i][0] >= max(color_detection_list[i][1],color_detection_list[i][2]) :
                colors[i] = "red"
            elif color_detection_list[i][1] >= max(color_detection_list[i][0],color_detection_list[i][2]) :
                colors[i] = "green"
            else : 
                colors[i] = "blue"
            cury = nexy
            imgs.append(cropped1)
        # Updates display
        # color_display_1.display(color_detection_list)
        print(colors)
        # print(len(imgs))
        cv2.imshow('img1', imgs[0])
        cv2.imshow('img2', imgs[1])
        cv2.imshow('img3', imgs[2])
        cv2.imshow('img4', imgs[3])        
        if cv2.waitKey(10) & 0xFF == ord("q"):  # waits for 'q' key to be pressed
            break

    cv2.destroyAllWindows()
