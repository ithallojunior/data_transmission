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
freq = [0., 0.] # to start with
t_freq_checker = 2. # checks frequency every N seconds
time_window = samples/fs # seconds
to_show = 1000
t = np.arange(0., time_window, 1./fs)
y1 = np.zeros(samples)
y2 = np.zeros(samples)


t1 = time.time()
while(1):
    try:
        plt.clf()
        plt.xlabel("Time(s)")
        plt.xlim(0, t[to_show])
        plt.ylim(-0.1, 1.2)#(-0.1, 1.3)

        i = 0; 
        while i < samples:
            value_1a = p.read()
            value_1b = p.read()
            value_2a = p.read()
            value_2b = p.read()
            #print "read"
            try:
                v1 = ord(value_1a[0])*256 + ord(value_1b[0])
                v2 = ord(value_2a[0])*256 + ord(value_2b[0])
                y1[i] = float(v1)*(1.1/1023.0)
                y2[i] = float(v2)*(1.1/1023.0)
                i = i +1
                #print "Values: ", v, ord(value1), ord(value2)
            except IndexError:
                print "Error"
                pass
        #print("Evlapsed time %f"%(time.time() -t1))
        plt.plot(t, y1, c="b", label="Channel 0")
        plt.plot(t, y2, c="r", label="Channel 1")
        plt.title("Fs: %.3f | F.ch0: %.3f | F.ch1: %.3f "%(fs, freq[0], freq[1]))
        plt.legend(loc="best")
        plt.grid()
        plt.pause(1.0/30.0)
        #print("time %f"%(time.time() -t1))
        #updates freq
        if ((time.time() - t1) >= t_freq_checker): #TODO check div zero
            y1mean = y1 - y1.mean() #removes DC
            fd1 = ( np.abs( np.fft.fft(y1mean) ) )[:samples/2].argmax()
            
            y2mean = y2 - y2.mean() #removes DC
            fd2 = ( np.abs( np.fft.fft(y2mean) ) )[:samples/2].argmax()
            freq = [round(fs * fd1/samples), round(fs * fd2/samples)]
            t1 = time.time()
    
    except KeyboardInterrupt:
        plt.close()
        break


p.close()
