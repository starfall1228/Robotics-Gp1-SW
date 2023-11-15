import cv2
import numpy as np
# Select your default camera.
cap = cv2.VideoCapture(0) # Change 0 if you have more than one camera.
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 720)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)

while True:
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
        elif hue_value < 22:
            color[i] = "ORANGE"
        elif hue_value < 33:
            color[i] = "YELLOW"
        elif hue_value < 78:
            color[i] = "GREEN"
        elif hue_value < 131:
            color[i] = "BLUE"
        elif hue_value < 170:
            color[i] = "VIOLET"
        else:
            color[i] = "RED"

        pixel_center_bgr = frame[cy, cx]
        b, g, r = int(pixel_center_bgr[0]), int(pixel_center_bgr[1]), int(pixel_center_bgr[2])

        # cv2.rectangle(frame, (cx - 220, 10), (cx + 200, 120), (255, 255, 255), -1)
        # cv2.putText(frame, color, (cx - 200, 100), 0, 3, (b, g, r), 5)
        # cv2.circle(frame, (cx, cy), 5, (25, 25, 25), 3)
    print(color)
    cv2.imshow('img1', imgs[0])
    cv2.imshow('img2', imgs[1])
    cv2.imshow('img3', imgs[2])
    cv2.imshow('img4', imgs[3])        

    key = cv2.waitKey(1)
    if key == 27:
        break

cap.release()
cv2.destroyAllWindows()