# -*- coding: utf-8 -*-
"""
Attiny85 calibrator helper, as the name says, helps
calibrating the internal clock by measuring the
actual frequency send by the oscillator.
It must be used with one device at a time, and the tiny must
have the oscillation code.
Author: Ithallo Junior Alves Guimarães
May 2020
"""
import serial
import matplotlib.pyplot as plt
import numpy as np
import time
import os
import settings
import modules


FREQUENCY = 4000.  # Hz
MIN_FREQUENCY_TO_SHOW = 0.  # Hz
MAX_FREQUENCY_TO_SHOW = FREQUENCY/2.
SAMPLES = 8192


def attiny85_callibrator():
    """"
    Shows the frequency and the adjustment  to be done on the Attiny85 or alike.
    """

    os.system("clear")
    print("Starting attiny85 calibrator")
    p = serial.Serial(port=settings.device, baudrate=settings.baud_rate,
                      bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE,
                      timeout=settings.timeout
                      )
    p.flush()

    time.sleep(0.5)

    plt.ion()
    plt.figure(figsize=(14, 8))

    grid_spacing = np.arange(
        MIN_FREQUENCY_TO_SHOW,
        MAX_FREQUENCY_TO_SHOW,
        (MAX_FREQUENCY_TO_SHOW-MIN_FREQUENCY_TO_SHOW)/20
    )
    samples = SAMPLES
    frequencies = np.fft.fftfreq(SAMPLES)
    f = abs(frequencies * FREQUENCY)
    y = np.zeros(samples)

    break_out = False  # if any error, breaks the loop
    while(1):
        try:
            plt.clf()
            plt.xlabel("Frequency(Hz)")
            plt.xlim(MIN_FREQUENCY_TO_SHOW, MAX_FREQUENCY_TO_SHOW)

            i = 0
            while i < samples:

                v1 = p.read()
                v2 = p.read()
                value = modules.convert_input(v1, v2)

                if (value is None):  # case data is being hung
                    break_out = True
                    print ("\nNo data, check your circuits.\n")
                    break
                else:
                    y[i] = value

                if (break_out):
                    break
                else:
                    i = i + 1

            if (break_out):
                break
            else:

                Y = np.abs(np.fft.fft(y - y.mean()))

                # getting the frequency
                freq_hz = round(abs(frequencies[Y.argmax()] * FREQUENCY), 2)

                plt.plot(f, Y, c='b', label='Signal')
                plt.xticks(grid_spacing)
                plt.grid(color='k', linestyle='-', linewidth=0.1)
                plt.legend(loc="upper right")
                plt.title(
                    "Signal freq.: %sHz || Fs: %.2fHz" % (freq_hz, FREQUENCY)
                )
                plt.pause(1.0/60.0)
        except KeyboardInterrupt:
            plt.close()
            break

    p.close()


if (__name__ == "__main__"):
    attiny85_callibrator()
