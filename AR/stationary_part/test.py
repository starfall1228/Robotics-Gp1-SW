# Python program to identify 
#color in images 

# Importing the libraries OpenCV and numpy 
import cv2 
import numpy as np 

# Read the images 
img = cv2.imread("shapes.jpg") 

# Resizing the image 
image = cv2.resize(img, (700, 600)) 

# Convert Image to Image HSV 
hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV) 

# Defining lower and upper bound HSV values 
lower = np.array([50, 100, 100]) 
upper = np.array([70, 255, 255]) 

# Defining mask for detecting color 
mask = cv2.inRange(hsv, lower, upper) 

# Display Image and Mask 
cv2.imshow("Image", image) 
cv2.imshow("Mask", mask) 

# Make python sleep for unlimited time 
cv2.waitKey(0) 
