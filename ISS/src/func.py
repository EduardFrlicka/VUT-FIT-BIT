import matplotlib.pyplot as plt
from numpy.core.fromnumeric import argmax
from scipy import signal
import numpy as np
from scipy.io.wavfile import read
from math import exp, pi

# A,B - waves, sample_size - in ms, sample_step - in ms
def get_most_alike_samples(A,B,sample_size, sample_step, RATE):
    sample_size = int(RATE / 1000 * sample_size)
    sample_step = int(RATE / 1000 * sample_step)

    max_i = len(A) - sample_size
    max_j = len(B) - sample_size
    
    max_cc = 0
    store_i = 0
    store_j = 0

    # i = 0
    # while i < max_i:
    #     j = 0
    #     while j < max_j:
    #         cc = np.corrcoef(A[i:i+sample_size], B[j:j+sample_size])[0,1]
    #         if(cc > max_cc):
    #             max_cc=cc
    #             store_i = i
    #             store_j = j
    #         j+=sample_step
    #     i+= sample_step

    # print(store_i, store_j, max_cc)

    store_i = 59200
    store_j = 4800

    return A[store_i: store_i + sample_size], B[store_j:store_j+sample_size]


def wave_to_frames(wave, frame_size, frame_step, RATE):
    frame_size = int(RATE / 1000 * frame_size)
    frame_step = int(RATE / 1000 * frame_step)

    max_pos = len(wave) - frame_size
    result = []

    pos = 0
    while pos <= max_pos:
        result.append(wave[pos:pos+frame_size])
        pos+= frame_step
    
    return result


def center_clipping(wave):
    ceil = np.abs(wave).max()*0.7
    return [ int(v/abs(v)) if (abs(v) > ceil) else 0 for v in wave ]

def autocorelation_P(wave):
    result = []
    for k in range(len(wave)):
        r = 0
        for n in range(0, len(wave)-1-k):
            r+=wave[n]*wave[n+k]
        result.append(r)
    return result

def DFT(array, N):
    array = np.pad(array,N)
    result = []
    for n in range(N):
        sum = 0.0 + 0j
        const = - 2j * pi * n / N
        for k in range(N):
            sum += array[k] * exp( k * const)
        result.append(sum)
    return result

def IDFT(array, N):
    array = np.pad(array,N)
    result = []
    for n in range(N):
        sum = 0.0 + 0j
        const = 2j * pi * n / N
        for k in range(N):
            sum += array[k] * exp( k * const)
        result.append(sum / N)
    return result