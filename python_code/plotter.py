# -*- coding: utf-8 -*-
"""
Plotter, it must be set with the settings.py file
Author: Ithallo Junior Alves Guimarães
March 2019
"""
import serial
import matplotlib.pyplot as plt
import numpy as np
from scipy import signal
import time
import os
import settings
import modules

def run_plotter():
    os.system("clear")
    print("Starting plotter...")
    p = serial.Serial(port=settings.device, baudrate=settings.baud_rate,
                      bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE, timeout=settings.timeout)
    p.flush()
    
    time.sleep(0.5)
    
    plt.ion()
    plt.figure(figsize = (14,8))
    
    grid_spacing = np.arange(0, settings.time_window_to_show, settings.time_window_to_show/settings.xticks)
    samples = int(settings.sampling_frequency * settings.time_window) #samples
    t = np.arange(0., settings.time_window, 1./settings.sampling_frequency)
    y = np.zeros((samples, settings.number_of_channels))
    
    
    # filter, getting the coefficients one time for all, as it is kinda slow
    if(settings.use_filter):
        b, a = signal.butter(settings.order, settings.fc, fs=settings.sampling_frequency, btype=settings.type, analog=False)

    
    break_out = False # if any error, breaks the loop
    while(1):
        try:
            plt.clf()
            plt.xlabel("Time(s)")
            plt.xlim(0, settings.time_window_to_show)
            if (not settings.remove_mean):
                plt.ylim(-1.05 * settings.offset , 1.05 * (settings.voltage_range - settings.offset))

            i = 0; 
            while i < samples:
                for j in range(settings.number_of_channels):
                    
                    v1 = p.read()
                    v2 = p.read()
                    value  = modules.convert_input(v1, v2)
                    
                    if (value is None):# case data is being hung
                        break_out = True
                        print ("\nNo data, check your circuits and run again.\n")
                        break
                    else:
                        y[i, j] = value 
                
                if (break_out):
                    break
                else:
                    i = i + 1
                    
            if (break_out):
                break
            else:
                for j in range(settings.number_of_channels):
                    
                    # removing DC
                    if (settings.remove_mean):
                        yn = y[:, j] - y[:, j].mean()
                    else:
                        yn = y[:, j]
                        
                   # filtering signal
                    if (settings.use_filter):
                        yn = signal.lfilter(b, a, yn)

                    plt.plot(t, yn, c=settings.colors[j], label="Channel %s"%(j+1))
                
                plt.xticks(grid_spacing)
                plt.grid(color='k', linestyle='-', linewidth=.1)
                plt.legend(loc="upper right")
                plt.title("Signal(s) || Fs: %.3f"%settings.sampling_frequency)
                plt.pause(1.0/30.0)
        except KeyboardInterrupt:
            plt.close()
            break


    p.close()



if (__name__=="__main__"):
    run_plotter()
