# Import the necessary modules
import cv2
import numpy as np

# Define the lower and upper boundaries of the red color in the HSV color space

# Define the lower and upper boundaries of the black color in the HSV color space

lower_green = np.array([36, 25, 25])
upper_green = np.array([70, 255, 255])

lower_red = np.array([0, 50, 50])
upper_red = np.array([10, 255, 255])

lower_blue = np.array([100, 50, 50])
upper_blue = np.array([130, 255, 255])
# Initialize the video capture object
cap = cv2.VideoCapture(1)

cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
# Loop until the user presses 'q' to quit
while True:
    # Read a frame from the camera
    ret, frame = cap.read()

    # Convert the frame to HSV color space
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # Create a mask for the red color
    # mask_red = cv2.inRange(hsv, lower_red, upper_red)

    # Create a mask for the black color
    # mask_green = cv2.inRange(hsv, lower_green, upper_green)
    mask_blue = cv2.inRange(hsv, lower_blue, upper_blue)
    mask_red = cv2.inRange(hsv, lower_red, upper_red)
    # Combine the two masks using the bitwise OR operation
    # mask = cv2.bitwise_or(mask_red, mask_black)

    # Find the contours of the red
    contours_red, hierarchy_red = cv2.findContours(
        mask_red, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    # contours_green, hierarchy_green = cv2.findContours(
        # mask_green, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    contours_blue, hierarchy_blue = cv2.findContours(
        mask_blue, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    # Draw the contours on the original frame
    cv2.drawContours(frame, contours_red, -1, (0, 255, 0), 2)
    # cv2.drawContours(frame, contours_green, -1, (0, 255, 0), 2)
    cv2.drawContours(frame, contours_blue, -1, (0, 255, 0), 2)

    # Show the frame
    cv2.imshow('Frame', frame)

    # Check if the mask contains any non-zero pixels
    if np.any(mask_red):
        # Print a message on the terminal
        print('red detected')
    # if np.any(mask_green):
    #     # Print a message on the terminal
    #     print('green detected')
    if np.any(mask_blue):
        # Print a message on the terminal
        print('blue detected')

    # Check if the user presses 'q' to quit
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release the video capture object and close all windows
cap.release()
cv2.destroyAllWindows()
