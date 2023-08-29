#!/bin/env python3

# pip install scikit-image

import time
import numpy as np
from scipy import ndimage as ndi
import math
from skimage.segmentation import watershed
from skimage.feature import peak_local_max
from scipy.stats import norm
import scipy

def gsmooth(I, s):
    L = math.ceil(4*s)
    x = np.linspace(-L, L, 2*L+1)
    K = norm.pdf(x, s)
    K = np.atleast_2d(K)
    #breakpoint()
    J = scipy.ndimage.convolve(I, K)
    J = scipy.ndimage.convolve(I, K.T)
    return J


def timeit(n):
    I = np.random.uniform(0, 1, [n, n])
    I = gsmooth(I,2)
    t0 = time.perf_counter()
    labels = watershed(I)
    t1 = time.perf_counter();
    print(f"n={n}, t={t1-t0} s")

if __name__ == "__main__":
    for e in range(8, 14):
        timeit(2**e)
