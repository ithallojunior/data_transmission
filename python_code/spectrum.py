# -*- coding: utf-8 -*-
"""
Spectrum plotter, it must be set with the settings.py file
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


def spectrum_plotter():
    os.system("clear")
    print("Starting spectrum plotter...")
    p = serial.Serial(port=settings.device, baudrate=settings.baud_rate,
                      bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE,
                      timeout=settings.timeout
                      )
    p.flush()

    time.sleep(0.5)

    plt.ion()
    plt.figure(figsize=(14, 8))

    grid_spacing = np.arange(
        0, settings.max_expected_frequency_to_show,
        settings.max_expected_frequency_to_show/settings.xticks
    )
    samples = settings.frequency_window
    frequencies = np.fft.fftfreq(samples)
    f = abs(frequencies * settings.sampling_frequency)
    y = np.zeros((samples, settings.number_of_channels))

    # filter, getting the coefficients one time for all, as it is kinda slow
    if(settings.use_filter):
        b, a = signal.butter(
            settings.order, settings.fc,
            fs=settings.sampling_frequency, btype=settings.type, analog=False
        )
    break_out = False  # if any error, breaks the loop
    while(1):
        try:
            plt.clf()
            plt.xlabel("Frequency(Hz)")
            plt.xlim(0, settings.max_expected_frequency_to_show)
           # plt.ylim(-1.05 * settings.offset , 1.05 *
           # (settings.voltage_range - settings.offset))

            i = 0
            while i < samples:
                for j in range(settings.number_of_channels):

                    v1 = p.read()
                    v2 = p.read()
                    y[i, j] = modules.convert_input(v1, v2)
                    i = i + 1

            for i in range(settings.number_of_channels):

                # removing DC
                if (settings.remove_mean):
                    yn = y[:, i] - y[:, i].mean()
                else:
                    yn = y[:, i]

                # filtering signal
                if (settings.use_filter):
                    yn = signal.lfilter(b, a, yn)

                Y = np.abs(np.fft.fft(yn))

                freq_hz = round(
                    abs(frequencies[Y.argmax()] * settings.sampling_frequency),
                    2
                )

                plt.plot(
                    f, Y, c=settings.colors[i],
                    label="Channel %s, Fp: %s Hz" % (j+1, freq_hz)
                )

            plt.xticks(grid_spacing)
            plt.grid(color='k', linestyle='-', linewidth=0.1)
            plt.legend(loc="upper right")
            plt.title("Frequency domain || Fs: %.3f  || Filter %s" % (
                      settings.sampling_frequency,
                      'ON' if settings.use_filter else 'OFF')
                      )
            plt.pause(1.0/60.0)
        except KeyboardInterrupt:
            plt.close()
            break

    p.close()


if (__name__ == "__main__"):
    spectrum_plotter()
