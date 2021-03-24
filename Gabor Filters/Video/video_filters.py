import cv2
import numpy as np
import matplotlib.pyplot as plt

# Filter bank creation
ksize = 15
sigma = 0.7
theta = 2 * np.pi / 3
lamda = 5 * np.pi / 4
# aspect ratio
gamma = 0.3
phi = 0

kernel = [
    cv2.getGaborKernel((ksize, ksize), sigma, theta, lamda, gamma, phi, ktype=cv2.CV_32F),
    cv2.getGaborKernel((ksize, ksize), sigma, -theta, lamda, gamma, phi, ktype=cv2.CV_32F),
    cv2.getGaborKernel((ksize, ksize), sigma, 1, lamda, gamma, phi, ktype=cv2.CV_32F),
    cv2.getGaborKernel((ksize, ksize), sigma, -1, lamda, gamma, phi, ktype=cv2.CV_32F),
    cv2.getGaborKernel((ksize, ksize), sigma, 1.5, lamda, gamma, phi, ktype=cv2.CV_32F),
    cv2.getGaborKernel((ksize, ksize), sigma, 0.5, lamda, gamma, phi, ktype=cv2.CV_32F),
    cv2.getGaborKernel((ksize, ksize), sigma, 2.5, lamda, gamma, phi, ktype=cv2.CV_32F),
    cv2.getGaborKernel((ksize, ksize), sigma, -2.5, lamda, gamma, phi, ktype=cv2.CV_32F),
    cv2.getGaborKernel((ksize, ksize), sigma, 0.5, lamda, gamma, phi, ktype=cv2.CV_32F),
    cv2.getGaborKernel((ksize, ksize), sigma, -0.5, lamda, gamma, phi, ktype=cv2.CV_32F),
    cv2.getGaborKernel((ksize, ksize), sigma, 1.2, lamda, gamma, phi, ktype=cv2.CV_32F),
    cv2.getGaborKernel((ksize, ksize), sigma, -1.2, lamda, gamma, phi, ktype=cv2.CV_32F),
    cv2.getGaborKernel((ksize, ksize), sigma, 1.4, lamda, gamma, phi, ktype=cv2.CV_32F),
    cv2.getGaborKernel((ksize, ksize), sigma, -1.4, lamda, gamma, phi, ktype=cv2.CV_32F),
    cv2.getGaborKernel((ksize, ksize), sigma, 0.4, lamda, gamma, phi, ktype=cv2.CV_32F),
    cv2.getGaborKernel((ksize, ksize), sigma, -0.4, lamda, gamma, phi, ktype=cv2.CV_32F),
]

