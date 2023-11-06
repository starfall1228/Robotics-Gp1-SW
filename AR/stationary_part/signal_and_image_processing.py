# Import the necessary modules
import cv2
import numpy as np

# Define the lower and upper boundaries of the red color in the HSV color space
lower_red = np.array([0, 100, 100])
upper_red = np.array([10, 255, 255])

# Define the lower and upper boundaries of the black color in the HSV color space
lower_black = np.array([0, 0, 0])
upper_black = np.array([180, 255, 30])

# Initialize the video capture object
cap = cv2.VideoCapture(0)

# Loop until the user presses 'q' to quit
while True:
    # Read a frame from the camera
    ret, frame = cap.read()

    # Convert the frame to HSV color space
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # Create a mask for the red color
    # mask_red = cv2.inRange(hsv, lower_red, upper_red)

    # Create a mask for the black color
    mask = cv2.inRange(hsv, lower_red, upper_red)

    # Combine the two masks using the bitwise OR operation
    # mask = cv2.bitwise_or(mask_red, mask_black)

    # Find the contours of the red and black regions
    contours, hierarchy = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    # Draw the contours on the original frame
    cv2.drawContours(frame, contours, -1, (0, 255, 0), 2)

    # Show the frame
    cv2.imshow('Frame', frame)

    # Check if the mask contains any non-zero pixels
    if np.any(mask):
        # Print a message on the terminal
        print('red detected')

    # Check if the user presses 'q' to quit
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release the video capture object and close all windows
cap.release()
cv2.destroyAllWindows()
