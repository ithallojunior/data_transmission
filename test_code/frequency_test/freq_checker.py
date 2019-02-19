import serial
import matplotlib.pyplot as plt
import numpy as np
import time
#import pdb
#pdb.set_trace
pd = '/dev/cu.usbmodem1411'
def worker():
    p = serial.Serial(port= pd, baudrate=115200,
                      bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE, timeout=None)
    p.flush()
    time.sleep(0.1)

    #plt.axis([0, 10, 0, 1])
    plt.ion()
    samples = 2048
    y = np.zeros(samples)
    i = 0
    fs  = 2048.0 # sampling frequency
    f_ext = 60.0#59.96 # frequency being measured
    
    xtick= np.linspace(-fs/2., fs/2., 11)
    f = np.linspace(-fs/2., fs/2., samples)
    print "starting loop"
    while(1):
        try:
            plt.clf()
            plt.xlabel("Freq(Hz)")
            #plt.xticks(xtick)
            plt.xlim(40, 256)
            #plt.ylim(-10, 100)
            i = 0
            while i < samples:
                value1 = p.read()
                value2 = p.read()
                try:
                    v = ord(value1[0])*256 + ord(value2[0])
                    y[i] = float(v)*(5./1023.0)
                    i = i +1
                except ValueError: pass
            y = y - y.mean() 
            Y = np.abs(np.fft.fft(y))
            fd = Y[:samples/2].argmax()
            Y = np.fft.fftshift(Y)
            plt.title("Fd: %s | Meas. F: %.3f Hz | Actual F: %.3f \n Expec. Fs: %.3f | Actual Fs: %.3f Hz"%(fd, fs * fd/samples, f_ext, fs, f_ext * samples/fd ))
            plt.plot(f, Y)
            #print time.time() - st
            plt.pause(1.0/15.)
        

        except KeyboardInterrupt:
            plt.close()
            p.flush()
            p.close()
            break


worker()





