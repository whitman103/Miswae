import pydicom
import os
import matplotlib.pyplot as plt
import pywt
import numpy as np
from pywt import coeffs_to_array
from copy import deepcopy as dc
import cv2
from compress_utils import EncodingMetadata
from torch import tensor, int16

import torchac

import blitzTry

basePath = '/Users/johnwhitman/Downloads/DICOM/ST000000/SE000000'

lenaImage = cv2.imread('/Users/johnwhitman/Downloads/lena.png')
lenaImage = cv2.cvtColor(lenaImage, cv2.COLOR_RGB2GRAY).astype('int32')

from compress_utils import padImage

files = os.scandir(basePath)
filePaths = [x.path for x in files]

test = pydicom.dcmread(filePaths[0])
baseData = test.pixel_array.data

baseImage = dc(lenaImage)

baseData = padImage(baseData)
imgs = [pow(2,i) * ((baseData& (1<<i)) >>i) for i in range(8)]
""" plt.imshow(baseData)
plt.show() """
print(baseData.dtype)
test = dc(baseData)
baseData = baseData.astype('int32')

blitzTry.decompose(lenaImage)

show = False
if show:
    plt.imshow(lenaImage.astype('int16'))
    plt.show()


flatLena = lenaImage.flatten()
negShift = min(flatLena)
flatLena-=negShift
symbols = list(set(np.unique(flatLena)))

pdf = [(value, np.count_nonzero(flatLena==value)) for value in set(np.unique(flatLena))]
pdf.sort(key = lambda x: x[1], reverse=True)

symbols = np.zeros(len(pdf)+1, dtype='int16')
cdf = np.zeros(len(pdf), dtype = 'int32')
runningCount = 0
for indexValue, (symbol, count) in enumerate(pdf):
    symbols[indexValue] = symbol
    cdf[indexValue]=runningCount
    runningCount+=count
cdf[-1]=0
cdfTensor = tensor(cdf)
flatTensor = tensor(flatLena,dtype=int16)
print(cdfTensor.shape,flatTensor.shape)


bytes_stream = torchac.encode_int16_normalized_cdf(tensor(cdf), tensor(flatLena[:len(cdf)-1],dtype=int16))
    
    
print(pdf)



blitzTry.recompose(lenaImage)
if show:
    plt.imshow(lenaImage)
    plt.show()
