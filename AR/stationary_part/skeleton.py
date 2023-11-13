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


if __name__ == "__main__":
    # TODO: Change your team's name
    color_display_1 = ColorDisplayWindow(
        group_name="Isabella One",
    )

    color_detection_list = [[0, 0, 0], [0, 0, 0], [0, 0, 0], [0, 0, 0]]
    
    cap = cv2.VideoCapture(0) # Change 0 if you have more than one camera.
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

        # Updates display
        _, frame = cap.read()
        hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        height, width, _ = frame.shape
        imgs = []
        cury = 0
        part = int(width / 4)
        party = int(height/4)
        color = ["","","",""]
        for i in range(4):
            nexy = min(cury + part,width)
            cropped1 = frame[0:height - 1, cury:nexy - 1]
            cury = nexy
            imgs.append(cropped1)
            cx = part // 2
            cy = height // 2

            pixel_center = hsv_frame[cy, cx+part*i]
            hue_value = pixel_center[0]

            # color = "Undefined"
            if hue_value < 5:
                color[i] = "RED"
                color_detection_list[i] = [0,0,255]
            elif hue_value < 22:
                color[i] = "ORANGE"
                color_detection_list[i] = [0,165,255]
            elif hue_value < 33:
                color_detection_list[i] = [0,255,255]
                color[i] = "YELLOW"
            elif hue_value < 78:
                color[i] = "GREEN"
                color_detection_list[i] = [0,255,0]
            elif hue_value < 131:
                color_detection_list[i] = [255,0,0]
                color[i] = "BLUE"
            elif hue_value < 170:
                color_detection_list[i] = [228,130,238]
                color[i] = "VIOLET"
            else:
                color_detection_list[i] = [0,0,255]
                color[i] = "RED"

            pixel_center_bgr = frame[cy, cx]
            b, g, r = int(pixel_center_bgr[0]), int(pixel_center_bgr[1]), int(pixel_center_bgr[2])

            # cv2.rectangle(frame, (cx - 220, 10), (cx + 200, 120), (255, 255, 255), -1)
            # cv2.putText(frame, color, (cx - 200, 100), 0, 3, (b, g, r), 5)
            # cv2.circle(frame, (cx, cy), 5, (25, 25, 25), 3)
        color_display_1.display(color_detection_list)
        print(imgs[0])
        # print(color)
        # cv2.imshow('img1', imgs[0])
        # cv2.imshow('img2', imgs[1])
        # cv2.imshow('img3', imgs[2])
        # cv2.imshow('img4', imgs[3])        

        key = cv2.waitKey(1)
        if key == 27:
            break
        if cv2.waitKey(10) & 0xFF == ord("q"):  # waits for 'q' key to be pressed
            break

    cv2.destroyAllWindows()