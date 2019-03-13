# -*- coding: utf-8 -*-
"""
Configuration file for the plotters
Author: Ithallo Junior Alves Guimarães
March 2019
"""
# serial settings
device = "/dev/cu.wchusbserial1410" # change accordingly
baud_rate = 115200 # depends on your arduino code
timeout = 5. # seconds, in order to prevent  the code from holding 

# frequency settings
sampling_frequency = 2040. # Hz
time_window = 1. # second(s)
time_window_to_show = 100e-3
frequency_window = 4096 # samples
total_time = 1.*60. # seconds, to be used to get the total number of samples
max_expected_frequency = 500.# Hz
max_expected_frequency_to_show = 2. * max_expected_frequency # Hz

time_to_update_frequency = 2. # every N seconds

# signal settings
voltage_range = 1.1 # max selected in the attiny85 sampler
offset = 0.5 # volt(s), set in the signal generator
remove_mean = True # case the 60Hz starts messing with the signal, removes the DC by the mean. If true it neglects the offset value
amplitude = 1. # volt(s), set in the signal generator

# data saving settings 
format = "%.4f" # I think 4 values after the point is enough

# transmitter settings
number_of_channels = 1  # number of channels to be displayed/sampled  

#plotting settings
colors = ["b", "r", "g", "y"]
xticks = 10
