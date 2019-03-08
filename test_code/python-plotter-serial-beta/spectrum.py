import serial
import matplotlib.pyplot as plt
#import multiprocessing
import numpy as np
import time
#import pdb
#pdb.set_trace
#pd = '/dev/cu.usbmodem1421'
pd ='/dev/cu.wchusbserial1410'
def worker():
    p = serial.Serial(port= pd, baudrate=115200,
                      bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE, timeout=None)
    #plt.axis([0, 10, 0, 1])
    plt.ion()
    samples = 2048
    y = np.zeros(samples)
    i = 0
    freq  = 2000.0
    xtick= np.linspace(-500, 500, 11)
    f = np.linspace(-float(freq)/2, float(freq)/2, samples)
    while(1):
  
        try:
            plt.clf()
            plt.xlabel("Freq(Hz)")
            #plt.xlim(100, 200)
            #plt.xticks(xtick)
            plt.xlim(0, 1000)
            #plt.ylim(-10, 100)
            i = 0
            while i < samples:
                value1 = p.read()
                value2 = p.read()
                try:
                    v = ord(value1[0])*256 + ord(value2[0])
                    y[i] = float(v)*(1.1/1023.0)
                    i = i +1
                except ValueError: pass
        
            Y = np.abs(np.fft.fftshift(np.fft.fft(y)))
            #Y = np.abs(np.fft.fft(y))
            #st = time.time()
           #f = np.linspace(0, freq, samples)
            plt.plot(f, Y)
            #print time.time() - st
            plt.pause(1.0/15.0)
        

        except KeyboardInterrupt:
            plt.close()
            p.close()
            break


worker()





