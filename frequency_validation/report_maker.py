# -*- coding: utf-8 -*-
"""
This code works as a report maker for the EMG system (Arduino) frequency validation.
It must be configured via the settings.py file. Here, its
purpose is to validate the sampling frequency.

Author: Ithallo Junior Alves Guimaraães
Feb 2019
"""

import settings
import numpy as np 
import os
import matplotlib.pyplot as plt

def run():
    os.system("clear")
    print("Initiating.")
    
    # loading the signal and calculating the fft
    X = np.loadtxt(settings.data_to_validate)
    X_fft = np.abs(np.fft.fft(X)) # it goes along the last axis
    
    #till the half of frequencies
    X_fft2 = X_fft[:, :X_fft.shape[1]/2 ]

    # getting the digital frequency  (position)
    fd = X_fft2.argmax(axis=1)
    
    # calculating the actual frequencies
    f = settings.sampling_frequency * fd/(settings.sampling_frequency * settings.time_window)
    # calculating the sampling frequencies
    S = settings.expected_frequency * ((settings.sampling_frequency * settings.time_window)/fd)  
    # expected (actual) frequency vector 
    f_expected = settings.expected_frequency * np.ones(f.shape)
    # expected (actual) sampling frequency vector 
    S_expected = settings.sampling_frequency * np.ones(S.shape)
    
    # plotting signal frequency
    plt.figure(figsize = (14,8))

    plt.plot(f, c="b", label="Signal frequency")
    plt.plot(f_expected, c="r", label="Expected frequency")
    
    plt.ylim(0.98 * settings.expected_frequency , 1.02 * settings.expected_frequency)  
    plt.xlim(0, f.shape[0])

    plt.title("Signal frequency variation over time")
    plt.ylabel("Frequency(Hz)")
    plt.xlabel("Time(s)")
    
    plt.grid()
    plt.legend(loc="best")
    plt.savefig("signal_"+settings.data_to_validate[:-4]+".png")
    plt.close()
    
    # plotting sampling frequency
    plt.figure(figsize = (14,8))

    plt.plot(S, c="b", label="Sampling frequency")
    plt.plot(S_expected, c="r", label="Expected frequency")
    
    plt.ylim(0.98 * settings.sampling_frequency , 1.02 * settings.sampling_frequency)  
    plt.xlim(0, S.shape[0])

    plt.title("Sampling frequency variation over time")
    plt.ylabel("Frequency(Hz)")
    plt.xlabel("Time(s)")
    
    plt.grid()
    plt.legend(loc="best")
    plt.savefig("sampling_"+settings.data_to_validate[:-4]+".png")
    plt.close()
    #report
    print("Writing report.")
    
    mf = open("README.md", "w")
    mf.write("# Frequency Validation\n")
    mf.write("This is the validation  for the sampling frequency. This report was generated automatically.\n\n")
    mf.write("## Methods\n")
    mf.write("""A senoidal signal with a **%.3f Hz** frequency, amplitude of **%.2f V**, 
    and offset of **%.3fV**  was given to the transmitter and its frequency
    was measured at the receiver.\n\n The supposed sampling frequency was **%.3f Hz**, the
    signal was sampled for **%.3f seconds**, and the FFT (to calculate the 
    frequency) was calculated every **%.3f seconds**.\n\n In order 
    to calculate the sampling frequency, the signal frequency
    was supposed to be static  (even though it is widely known that it admits a little
    fluctuation, which is also shown in the results).\n\n The sampled signal was generated
    by a SIGLENT SDG 830 function generator.\n\n"""%(
            settings.expected_frequency, settings.amplitude, 
            settings.offset, settings.sampling_frequency, 
            settings.total_time * settings.time_window,
            settings.time_window)  )
    
    
    mf.write("## Results and discussion\n")
    
    mf.write("### Signal frequency variation\n")
    mf.write("The following table analyzes the signal frequency that was measured.\n\n")
    mf.write(" | Unit               |  Value     |\n")
    mf.write(" | ------------------ | :--------: |\n")
    mf.write(" | Expected frequency | **%.3fHz** |\n"%settings.expected_frequency)
    mf.write(" | Mean frequency ± std\*    | **%.3f±%.3fHz** |\n"%(f.mean(), f.std(ddof=1)))
    mf.write(" | Maximum measured frequency | **%.3fHz** |\n"%(f.max()))
    mf.write(" | Mininum measured frequency | **%.3fHz** |\n\n"%(f.min()))
    mf.write("\* Unbiased standard deviation. Same as Matlab's one.\n\n")

    mf.write("""It is important to notice that the maximum deviation from the 
    expected valued of ***%.3fHz*** was **%.3f%%** for values below it,
    **%.3f%%** for values above it, and **%.3f%%** for the mean. The following plot shows
    this variation\n\n."""%(settings.expected_frequency, 
    100. * (abs(f.min() - settings.expected_frequency)/settings.expected_frequency),
    100. * (abs(f.max() - settings.expected_frequency)/settings.expected_frequency),
    100. * (abs(f.mean() - settings.expected_frequency)/settings.expected_frequency) ))
    mf.write("![Plot of the measured signal frequencies.](signal_%s.png)\n\n"%settings.data_to_validate[:-4])


    mf.write("### Sampling frequency variation\n")
    mf.write("The following table analyzes the sampling frequency that was measured.\n\n")
    mf.write(" | Unit               |  Value     |\n")
    mf.write(" | ------------------ | :--------: |\n")
    mf.write(" | Expected frequency | **%.3fHz** |\n"%settings.sampling_frequency)
    mf.write(" | Mean frequency ± std\*    | **%.3f±%.3fHz** |\n"%(S.mean(), S.std(ddof=1)))
    mf.write(" | Maximum measured frequency | **%.3fHz** |\n"%(S.max()))
    mf.write(" | Minimim measured frequency | **%.3fHz** |\n\n"%(S.min()))
    mf.write("\* Unbiased standard deviation. Same as Matlab's one.\n\n")

    mf.write("""It is important to notice that the maximum deviation from the 
    expected valued of ***%.3fHz*** was **%.3f%%** for values below it,
    **%.3f%%** for values above it, and **%.3f%%** for the mean. The following plot shows
    this variation\n\n."""%(settings.expected_frequency, 
    100. * (abs(S.min() - settings.sampling_frequency)/settings.sampling_frequency),
    100. * (abs(S.max() - settings.sampling_frequency)/settings.sampling_frequency),
    100. * (abs(S.mean() - settings.sampling_frequency)/settings.sampling_frequency) ))
    mf.write("![Plot of the measured sampling frequencies.](sampling_%s.png)\n"%settings.data_to_validate[:-4])

    mf.close()
    
    if(settings.pandoc_update):
        print("Updating pandoc generated PDF report.")
        os.system("pandoc README.md -o report.pdf")

    print("The report is finished")

if (__name__ == "__main__"):
    run()
