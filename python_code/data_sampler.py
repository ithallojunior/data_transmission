# -*- coding: utf-8 -*-
"""
This code works as a sampler for the EMG system (Arduino).
It must be configured via the settings.py file. Here, its
purpose is to get the data to validate the sampling frequency.

Author: Ithallo Junior Alves Guimarães
May 2019
"""
import settings
import modules
import numpy as np
import serial
import datetime
import os
import time


#the run of the code
def run():

    os.system("clear")
    raw_input("Press enter to start...")

    p  = serial.Serial(port=settings.device, baudrate=settings.baud_rate,
                        bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE, 
                        timeout=settings.timeout)
    #timeout to be blocking

    time.sleep(0.1) # settle time
    p.flush() # deletes buffers

    # control variables and max
    total_samples = int( settings.sampling_frequency * settings.total_time)
    delta_time = 1./settings.sampling_frequency
    broke_out = False   
    
    #allocating
    X = np.zeros((total_samples, 2))

    #sampling
    for i in xrange(total_samples):
        try:
            #reading  value
            a = p.read()
            b = p.read()

            #converting to 
            value = modules.convert_input(a,b)
            if (value is None):
                print("\nNo data, check your circuits and run again.\n")
                broke_out = True
                break
            else:
                X[i,0] = i * delta_time
                X[i,1] = value

            #loading screen
            percent = 100. * float(i)/total_samples
            #os.system("clear")
            print("Running --- %.3f%%"%percent )

        except KeyboardInterrupt:
                print("\nInterrupted\n")
                time.sleep(.5)
                broke_out = True
                break


    if (not broke_out):
        #saving to file
        text = "data_%s.txt"%(str(datetime.datetime.now())[:-7]).replace(" ", "_").replace(":", "-")
        np.savetxt(text, X, fmt=settings.format)
        print ("file saved to %s "%text)

    #closing port
    p.flush()
    p.close




if (__name__ == "__main__"):
    run()
