import serial
import matplotlib.pyplot as plt
import numpy as np
import time
pd = "/dev/tty.usbmodem3a21"


p = serial.Serial(port=pd, baudrate=230400,
                  bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE)
#plt.axis([0, 10, 0, 1])
plt.ion()

samples = 500
x = np.arange(samples)
y = np.zeros(samples)



while(1):
    try:
        plt.clf()
        plt.xlabel("Time(s)")
        plt.xlim(0, samples)
        plt.ylim(-0.1, 1.3)

        i = 0; t1 = time.time()
        while i < samples:
            value1 = p.read()
            value2 = p.read()

            try:
                v = ord(value1[0])*256 + ord(value2[0])
                y[i] = float(v)*(1.1/1023.0)
                x[i] = i
                i = i +1
                #print "Values: ", v, ord(value1), ord(value2)
            except IndexError:
                print "Error"
                pass
        print("Evlapsed time %f"%(time.time() -t1))
        plt.plot(x, y)
        plt.pause(1.0/60.0)
        print("time %f"%(time.time() -t1))
    except KeyboardInterrupt:
        plt.close()
        break


p.close()
