"""
This code tests the conversion times for the different ways of the data received
from the external device
Author:  Ithallo J. A. Guimaraes
March 2019
"""
import time
import numpy as np
import os

# measures conversion time  and return a numpy array
def straight( total=1000000):
    time_array = np.zeros(total)
    for i in xrange(total):
        st = time.time()
        int("1022\n"[:-1])
        time_array[i] = time.time() - st
    return time_array * 1e6 # return in useconds, its easier for really small times


def division(total=1000000):
    time_array = np.zeros(total)
    for i in xrange(total):
        st = time.time()
        ord("\x03") + ord("\xfe")
        time_array[i] = time.time() - st
    return time_array * 1e6 # return in useconds, its easier for really small times


def shift( total=1000000):
    time_array = np.zeros(total)
    for i in xrange(total):
        st = time.time()
        ord("\x03")<<8 + ord("\xfe")&255
        time_array[i] = time.time() - st
    return time_array * 1e6 # return in useconds, its easier for really small times

# stats calc
def stats(data):
    print("Mean: %.4f useconds"%(data.mean()))
    print("STD.: %.4f useconds"%(data.std(ddof=1)))
    print("Max.: %.4f useconds"%(data.max()))
    print("Min.: %.4f useconds\n\n"%(data.min()))


# the main run
def run():
    
    os.system(" clear")
    

    #straight 
    print("####Results for 1,000,000 runs####\n\n")
    print("Straight way timing:")
    stats( straight() )

    # division
    print("Timing using divison:")
    stats( division())

    # shift 
    print("Timing using shift:")
    stats( shift())




if (__name__=="__main__" ):
    run()
