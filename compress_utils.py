import numpy as np
from typing import List
from dataclasses import dataclass
 

def padImage(inputImage: np.ndarray = None) -> np.ndarray:
    padSizes = []
    for side in inputImage.shape:
        currentPower = 0
        while pow(2,currentPower)<side:
            currentPower+=1
        padSizes.append(pow(2,currentPower)-side)
    return np.pad(inputImage,pad_width=padSizes)

def createBitplanes(inputImage: np.ndarray = None) -> List[np.ndarray]:
    return [pow(2,i) * ((inputImage& (1<<i)) >>i) for i in range(8)]
    
    
@dataclass
class EncodingMetadata:
    negShift: int
    frequencyHist: dict