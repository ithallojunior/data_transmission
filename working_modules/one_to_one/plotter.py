import serial
import matplotlib.pyplot as plt
import numpy as np
import time
pd = "/dev/tty.wchusbserial1410"
#pd = "/dev/tty.usbmodem1421"

p = serial.Serial(port=pd, baudrate=115200,
                  bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE)
p.flush()
time.sleep(0.5)
#plt.axis([0, 10, 0, 1])
plt.ion()

samples = 1024#samples
fs = 2000. # samples/sec
freq = 0. # to start with
t_freq_checker = 2. # checks frequency every N seconds
time_window = samples/fs # seconds
to_show = 1000
t = np.arange(0., time_window, 1./fs)
y = np.zeros(samples)


t1 = time.time()
while(1):
    try:
        plt.clf()
        plt.xlabel("Time(s)")
        plt.xlim(0, t[to_show])
        plt.ylim(-0.1, 1.2)#(-0.1, 1.3)

        i = 0; 
        while i < samples:
            value1 = p.read()
            value2 = p.read()
            #print "read"
            try:
                v = ord(value1[0])*256 + ord(value2[0])
                y[i] = float(v)*(1.1/1023.0)
                i = i +1
                #print "Values: ", v, ord(value1), ord(value2)
            except IndexError:
                print "Error"
                pass
        #print("Evlapsed time %f"%(time.time() -t1))
        plt.plot(t, y)
        plt.title("Fs: %.3f | Rounded Freq.: %.3f "%(fs, freq))
        plt.grid()
        plt.pause(1.0/30.0)
        #print("time %f"%(time.time() -t1))
        #updates freq
        if ((time.time() - t1) >= t_freq_checker):
            ymean = y - y.mean() #removes DC
            fd = ( np.abs( np.fft.fft(ymean) ) )[:samples/2].argmax()
            freq = round(fs * fd/samples)
            t1 = time.time()
    except KeyboardInterrupt:
        plt.close()
        break


p.close()
