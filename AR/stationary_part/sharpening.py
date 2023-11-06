import cv2
import numpy as np

# create a VideoCapture object to access the camera
cap = cv2.VideoCapture (0) # use 0 for default camera

# create a sharpening kernel
kernel = np.array ( [ [-1,-1,-1], [-1,9,-1], [-1,-1,-1]])
lower_red = np.array([0, 100, 100])
upper_red = np.array([10, 255, 255])

lower_black = np.array([0, 0, 0])
upper_black = np.array([180, 255, 30])

lower_green = np.array([40, 40, 40])
upper_green = np.array([70, 255, 255])
# loop to read and process each frame
while True:
    # read a frame from the camera
    ret, frame = cap.read () # ret is True if the frame was read correctly
    if not ret:
        break # exit the loop if there is an error

    # apply the sharpening kernel to the frame
    sharpened = cv2.filter2D (frame, -1, kernel) # -1 means the same depth as the input
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv, lower_black, upper_black)
    # display the original and sharpened frames
    # cv2.imshow ('Original', frame) # show the original frame
    contours, hierarchy = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    cv2.drawContours(frame, contours, -1, (0, 255, 0), 2)
    cv2.imshow ('frame', frame) # show the sharpened frame
    
    if(np.any(mask)):
        print("black detected")
    # wait for a key press and exit the loop if 'q' is pressed
    key = cv2.waitKey (1) # wait for 1 millisecond and get the pressed key
    if key == ord ('q'): # if the user presses 'q', exit the loop
        break

# release the resources and destroy the windows
cap.release () # release the camera or the video file
cv2.destroyAllWindows () # close all the windows
