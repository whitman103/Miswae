import pydicom
import os
import numpy as np
import matplotlib.pyplot as plt

from copy import deepcopy as dc
from math import log2

import blitzTry
from compress import CompressionInterface
import pickle
import os

MammoBasePath = "/Users/johnwhitman/Projects/ImagingData/manifest-1684004400435/VICTRE/1785298621/01-06-2018-NA-DBT Reconstructed Volume-74209/DBT slices-53873"

CTBasePath = "/Users/johnwhitman/Projects/ImagingData/manifest-1684004400435/ACRIN-FLT-Breast/ACRIN-FLT-Breast_004/01-01-1960-345678-PET-CT STUDY-06404/3.000000-PET-CT SERIES-00748"

PETBasePath = "/Users/johnwhitman/Projects/ImagingData/manifest-1684004400435/ACRIN-FLT-Breast/ACRIN-FLT-Breast_007/12-28-1959-345678-PET-CT STUDY-90735/148830.000000-PET-CT SERIES-99733"

MRIBasePath = "/Users/johnwhitman/Projects/ImagingData/manifest-1684004400435/ACRIN-6698/ACRIN-6698-207837/12-06-2002-207837T0-ACRIN-6698ISPY2MRIT0-38495/251800.000000-ISPY2 VOLSER uni-lateral cropped original DCE-30528"

for path in [CTBasePath]:
    
    dcmFiles = [pydicom.dcmread(x.path) for x in os.scandir(path)]
    
    compiledImage = np.array([x.pixel_array for x in dcmFiles])
    

    
image = np.amax(compiledImage,axis=0)
print(image.shape)
image = image[232:(232+96),232:(232+96)].astype('int32')
blitzTry.decompose(image)

testImage = image.flatten()
shift = np.min(testImage)
testImage-=shift
print(log2(max(testImage)))




testCompressor = CompressionInterface(len(testImage))
compressed = testCompressor.compressVector(vector=testImage)


testImage.tofile('testImage.dat',sep="")
with open('compressedData.dat','wb') as outFile:
    outFile.write(compressed)
pickle.dump({'c':compressed,'t':testCompressor.symbolTable},open('compressedPickle.pkl','wb'))

print(f'Original file size is: {os.stat("testImage.dat").st_size}')
print(f'Original gzip size is: {os.stat("testImage.dat.gz").st_size}')
print(f'Compressed size is: {os.stat("compressedData.dat").st_size}')
print(f'Compression ratio is {os.stat("testImage.dat").st_size/os.stat("compressedData.dat").st_size}')
print(f'bpp is {os.stat("compressedData.dat").st_size/len(testImage)}')