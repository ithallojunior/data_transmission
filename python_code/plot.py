"""
Plots and saves a PNG file for the file passed as argument.
Usage: python plot.py --options
Options:
--save to save
--nomean to remove mean
"""

from sys import argv

import matplotlib.pyplot as plt
import numpy as np


def plot(filepath, *args):
    """
    This method plots a stream of two dimensional 
    points (x,y) and saves it as a PNG file if requested.
    """

    data = np.loadtxt(filepath)
    t = data[:, 0]
    y = data[:, 1]

    if '--nomean' in args:
        y = y - y.mean()

    plt.figure(figsize=(14, 8))
    # plt.ylim([-0.6, 0.6])  # to get it just like the previous
    plt.ylim([y.min(), y.max()])
    #plt.xlim([0., 4.])
    plt.plot(t, y, c="b", linewidth=2)
    #plt.rc('font', size=24)
    #plt.rc('axes', titlesize=24)

    plt.title("sEMG signal", size=20)
    plt.xlabel("Time (s)", size=20)
    plt.ylabel("Amplitude (V)", size=20)
    plt.grid()
    plt.show()

    if '--save' in args:
        plt.savefig(filepath.split('.')[0]+'.png')
        print('saved to file!')
    plt.close()


if (__name__ == "__main__"):

    try:
        filepath = argv[1]
    except IndexError:
        raise Exception('You must pass a filepath as argument')

    plot(filepath, *argv)
