"""
This code works as a sampler for the EMG system (Arduino).
It must be configured via the settings.py file.
After sampling, it calls the plotter, if the argument
'--plot' was passed. Other possible arguments may be passed,
as detailed in the 'plot.py file'.

"""

import os
import datetime
import time
from sys import argv

import serial
import numpy as np

import modules
import settings
from plot import plot


# the run of the code
def sampler():
    """Samples the sEMG signal as declared on the settings file."""

    os.system("clear")
    raw_input("Press enter to start...")

    p = serial.Serial(port=settings.device, baudrate=settings.baud_rate,
                      bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE,
                      timeout=settings.timeout)
    # timeout to be blocking

    time.sleep(0.1)  # settle time
    p.flush()  # deletes buffers

    # control variables and max
    total_samples = int(settings.sampling_frequency * settings.total_time)
    delta_time = 1./settings.sampling_frequency
    broke_out = False

    # allocating
    X = np.zeros((total_samples, 2))

    # sampling
    for i in xrange(total_samples):
        try:
            # reading  value
            a = p.read()
            b = p.read()

            # converting to
            value = modules.convert_input(a, b)
            if (value is None):
                print("\nNo data, check your circuits and run again.\n")
                broke_out = True
                break
            else:
                X[i, 0] = i * delta_time
                X[i, 1] = value

            # loading screen
            percent = 100. * float(i)/total_samples
            # os.system("clear")
            print("Running --- %.3f%%" % percent)

        except KeyboardInterrupt:
            print("\nInterrupted\n")
            time.sleep(.5)
            broke_out = True
            break

    if (not broke_out):
        # saving to file
        text = settings.default_path + "data_%s.txt" % (
            str(datetime.datetime.now())[:-7]
        ).replace(" ", "_").replace(":", "-")

        np.savetxt(text, X, fmt=settings.format)
        print ("file saved to %s " % text)

    # closing port
    p.flush()
    p.close

    return text


if (__name__ == "__main__"):

    filepath = sampler()
    plot(filepath, *argv)
