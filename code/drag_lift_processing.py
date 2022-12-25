from email.mime import image
import cv2
import matplotlib.pyplot as plt
import numpy as np
import sys
import math
from scipy.interpolate import interp1d
import pandas as pd

image_file = '../img/drag_lift.png'
# img = imp.get_image(image_file)

 
# Load the input image
image_color = cv2.imread(image_file,cv2.IMREAD_UNCHANGED)
cv2.imshow('color', image_color)
cv2.waitKey(0)
cv2.destroyAllWindows()
# Converts images from BGR to HSV
hsv = cv2.cvtColor(image_color, cv2.COLOR_BGR2HSV)
lower_blue = np.array([110,50,50])
upper_blue = np.array([130,255,255])
blue_mask = cv2.inRange(hsv, lower_blue, upper_blue)
blue_mask[int(blue_mask.shape[0]*0.9):int(blue_mask.shape[0]*0.93),int(blue_mask.shape[1]*0.6):int(blue_mask.shape[1]*0.65)] = 0

print(np.unique(blue_mask))
cv2.imshow('mask',blue_mask)
cv2.waitKey(0)
cv2.destroyAllWindows()


image_grey = cv2.imread(image_file, 0)
image_grey[image_grey > 200] = 255
image_grey[image_grey <= 200] = 0

image_grey[int(image_grey.shape[0]*0.7):int(image_grey.shape[0]*0.95),int(image_grey.shape[1]*0.5):int(image_grey.shape[1]*0.97)] = 255
image_grey[int(image_grey.shape[0]*0.44):int(image_grey.shape[0]*0.53),int(image_grey.shape[1]*0.5):int(image_grey.shape[1]*0.76)] = 255
image_grey[int(image_grey.shape[0]*0.1):int(image_grey.shape[0]*0.2),int(image_grey.shape[1]*0.4):int(image_grey.shape[1]*0.7)] = 255

# plt.hist(image_grey.ravel(),256,[0,256])
# plt.show()

image_binary = np.empty_like(image_grey)
image_binary[image_grey < 128] = 0
image_binary[image_grey >= 128] = 1
image_binary = 1 - image_binary
# print(image_grey)


# plt.hist(image_grey.ravel(),256,[0,256]); plt.show()

row_ans_bar = []
hori_bar = np.ones(image_grey.shape[1])


for i in range(image_binary.shape[0]):
    ans = hori_bar*image_binary[i]
    row_ans_bar.append(np.sum(ans,axis = -1))
row_ans_bar = np.array(row_ans_bar,dtype=int)
print(row_ans_bar)
row_indices = np.where(row_ans_bar > image_binary.shape[1]*0.7)[0]
lower_bound = np.max(row_indices[row_indices < image_binary.shape[0]*0.5])
upper_bound = np.min(row_indices[row_indices > image_binary.shape[0]*0.5])
print(lower_bound,upper_bound)

vert_bar = np.ones(image_grey.shape[0])
col_ans_bar = []

for i in range(image_binary.shape[1]):
    ans = vert_bar*image_binary[:,i]
    col_ans_bar.append(np.sum(ans,axis = -1))
col_ans_bar = np.array(col_ans_bar,dtype= int)
col_indices = np.where(col_ans_bar > image_binary.shape[0]*0.7)[0]

left_bound = np.max(col_indices[col_indices < image_binary.shape[1]*0.5])
right_bound = np.min(col_indices[col_indices > image_binary.shape[1]*0.5])



crop_image = image_grey[lower_bound+1: upper_bound, left_bound + 1: right_bound]
# crop_image = cv2.resize(crop_image, (601,crop_image.shape[0]))

blue_mask = blue_mask[lower_bound+1: upper_bound, left_bound + 1: right_bound]
blue_mask = cv2.resize(blue_mask, (601,blue_mask.shape[0]))
cv2.imshow('blue', blue_mask)
cv2.waitKey(0)
cv2.destroyAllWindows()

drag_image = blue_mask.copy()
drag_image[int(blue_mask.shape[0]*0.45):, :] = 0
cv2.imshow('drag', drag_image)
cv2.waitKey(0)
cv2.destroyAllWindows()
print("index:")
for j in range(drag_image.shape[1]):
    print(np.where(drag_image[:,j] == 255)[0])
lift_image = blue_mask.copy()
lift_image[:int(blue_mask.shape[0]*0.46), :] = 0
cv2.imshow('lift', lift_image)
cv2.waitKey(0)
cv2.destroyAllWindows()
print("drag_img:",np.unique(drag_image))
print("lift_img:",np.unique(lift_image))


value_list = []
for j in range(drag_image.shape[1]):
    value = np.mean(np.where(drag_image[:,j] == 255)[0])
    value_list.append(value)
print(value_list)

x_to_rotate = interp1d([0,drag_image.shape[1]-1],[0,6])
y_to_Cd = interp1d([0,drag_image.shape[0]],[0.8,0])

Cd_x_values = []
Cd_values = []
for index, value in enumerate(value_list):
    Cd_x_values.append(np.around(x_to_rotate(index),2))
    Cd_values.append(np.around(y_to_Cd(value),6))
Cd_values = pd.Series(Cd_values).fillna(method='ffill').tolist()
print(Cd_values)

value_list = []
for j in range(lift_image.shape[1]):
    value = np.mean(np.where(lift_image[:,j] == 255)[0])
    value_list.append(value)
x_to_rotate = interp1d([0,lift_image.shape[1]-1],[0,6])
y_to_Cl = interp1d([0,lift_image.shape[0]],[0.8,0])
x_values = []
Cl_values = []
for index, value in enumerate(value_list):
    # f.write(f'{np.around(x_to_rotate(index),2)} {np.around(y_to_Cd(value),6)}\n')
    x_values.append(np.around(x_to_rotate(index),2))
    Cl_values.append(np.around(y_to_Cl(value),6))# print(Cl_values)
Cl_values = pd.Series(Cl_values).fillna(method='ffill').tolist()
Cl_values = pd.Series(Cl_values).fillna(method='bfill').tolist()
print(Cl_values)

path_drag = 'Cdmap.txt'
f = open(path_drag, 'w')
for x, Cd in zip(Cd_x_values, Cd_values):
    # print(index,":",x_to_rotate(index),":",y_to_Cl(value))
    f.write(f'{x} {Cd}\n')
f.close()
path_lift = 'Clmap.txt'
f2 = open(path_lift, 'w')
for x, Cl in zip(x_values, Cl_values):
    # print(index,":",x_to_rotate(index),":",y_to_Cl(value))
    f2.write(f'{x} {Cl}\n')
f2.close()
