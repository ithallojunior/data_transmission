# -*- coding: utf-8 -*-
"""
This code works as a sampler for the EMG system (Arduino).
It must be configured via the settings.py file. Here, its
purpose is to get the data to validate the sampling frequency.

Author: Ithallo Junior Alves Guimarães
Feb 2019
"""
import validation_settings as settings
import numpy as np
import serial
import datetime
import os
import time

#converts the two bytes into a single number
def convert_input(a, b, max=1.1, offset=0. ):
    if (a=="" or b==""):
        return 1024 # beacuse it is non-blocking

    value = ord(a[0]) *256 + ord(b[0])

    if (max is not None):
        return  max * (value/1023.) - offset
    else:
        return value

#the run of the code
def run():

    os.system("clear")
    raw_input("Press enter to start...")

    p  = serial.Serial(port=settings.device, baudrate=settings.baud_rate,
                        bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE, timeout=5.)
    #timeout to be blocking

    time.sleep(0.1) # settle time
    p.flush() # deletes buffers

    # control variables and max
    absolute_max_count = int( settings.total_time/settings.time_window )
    window_max_count = int( settings.sampling_frequency * settings.time_window)
    max_max = absolute_max_count * window_max_count
    count = 0.
    break_out = False

    #allocating
    X = np.zeros((absolute_max_count, window_max_count))

    #sampling
    for i in xrange(absolute_max_count):
        for j in xrange(window_max_count):
            try:
                #reading  value
                a = p.read()
                b = p.read()

                #converting to int
                value = convert_input(a, b, settings.voltage_range, settings.offset)
                if (value ==1024):
                    print("\nNo data, check your circuits and run again.\n")
                    break_out = True
                    break
                else:
                    X[i,j] = value

                #loading screen
                count += 1.
                percent = 100. * count/max_max
                #os.system("clear")
                print("Running --- %.3f%%"%percent )

            except KeyboardInterrupt:
                    print("\nInterrupted\n")
                    time.sleep(.5)
                    break_out = True
                    break

        if (break_out):
            break

    if (not break_out):
        #saving to file
        text = "data_%s.txt"%(str(datetime.datetime.now())[:-7]).replace(" ", "_").replace(":", "-")
        np.savetxt(text, X, fmt=settings.format)
        print ("file saved as %s "%text)

    #closing port
    p.flush()
    p.close




if (__name__ == "__main__"):
    run()
