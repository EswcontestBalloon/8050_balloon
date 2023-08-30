import numpy as np
from os import listdir
import cv2
from PIL import Image

path = '/home/balloon/PycharmProjects/datasets/fire/train/images/'
new_path = '/home/balloon/PycharmProjects/datasets/fire/train_gray/'

for file in listdir(path):
    img = Image.open(path+file).convert('L')
    img_numpy = np.array(img, 'uint8')
    cv2.imwrite(new_path+file, img_numpy)
