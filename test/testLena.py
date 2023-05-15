import matplotlib.pyplot as plt
import cv2

import blitzInterface
import compress


lenaImage = cv2.imread('lena.png')
lenaImage = cv2.cvtColor(lenaImage, cv2.COLOR_RGB2GRAY).astype('int32')


baseImage = lenaImage[128:(128+32),128:(128+32)].astype('int32')

blitzInterface.decompose(baseImage)

shift = baseImage.min()
Compressor = compress.CompressionInterface(len(baseImage.flatten()))
baseImage-=shift
flatImage = baseImage.flatten()
compressedFile = Compressor.compressVector(vector = flatImage)
flatImage.tofile('fullImage.dat',sep="")
with open('compressedLena.dat','wb') as outFile:
    outFile.write(compressedFile)

