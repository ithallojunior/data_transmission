# -*- coding: utf-8 -*-
"""
Configuration file for the sampler.py
Author: Ithallo Junior Alves Guimarães
Feb 2019
"""
device = "/dev/cu.wchusbserial1410" # change accordingly
baud_rate = 115200 # depends on your arduino code

sampling_frequency = 2040. # Hz
time_window = 2. # second(s)
total_time = 5.*60 # seconds, to be used to get the total number of samples
expected_frequency = 500.# Hz

voltage_range = 1.1 # max selected in the attiny85 sampler
offset = 0.5 # volt(s), set in the signal generator
amplitude = 1. # volt(s), set in the signal generator

format = "%.4f" # I think 4 values after the point is enough

folder_to_save = "./device2/"#folder to save  depending on what devices
data_to_validate = "data_2019-03-14_17-05-13.txt" # change to actual file name
pandoc_update = True # change if you don't want the PDF/ don't have pandoc 

