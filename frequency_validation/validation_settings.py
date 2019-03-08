# -*- coding: utf-8 -*-
"""
Configuration file for the sampler.py
Author: Ithallo Junior Alves Guimarães
Feb 2019
"""
device = "/dev/cu.wchusbserial1410" # change accordingly
baud_rate = 115200 # depends on your arduino code

sampling_frequency = 2000. # Hz
time_window = 1. # second(s)
total_time = 5*60. # seconds, to be used to get the total number of samples
expected_frequency = 500.# Hz

voltage_range = 1.1 # max selected in the attiny85 sampler
offset = 0.5 # volt(s), set in the signal generator
amplitude = 1. # volt(s), set in the signal generator

format = "%.4f" # I think 4 values after the point is enough

data_to_validate = "data_2019-03-08_19-48-08.txt" #"data_2019-02-27_14-27-39.txt" # change to actual file name

pandoc_update = True # change if you don't want the PDF/ don't have pandoc 

