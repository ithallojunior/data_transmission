import serial
import matplotlib.pyplot as plt
import numpy as np
pd = '/dev/cu.usbmodem1421'

p = serial.Serial(port=pd, baudrate=230400,
                  bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE)
p.flushInput()
plt.ion()

samples = 512
x = np.arange(samples)
y = np.zeros(samples)
i = 0
level = np.ones(samples)

while(1):
    try:
        plt.clf()
        plt.xlabel("Time(s)")
        plt.xlim(0, samples)
        plt.ylim(-0.6, 0.6)
        i = 0
        while i < samples:
            v_control = p.read() #signal normally is not 0, if 0 go to next
            if(ord(v_control) == 0):
                value1 = p.read()
                value2 = p.read()

                try:
                    v = ord(value1[0])*256 + ord(value2[0])
                    y[i] = float(v)*(1.1/1023.0)
                    x[i] = i
                    i = i +1
                except IndexError:
                    pass
        
        y = y - y.mean() # no DC allowed D:
        #for i in x: removing noise in a bad way
            #if ((y[i] >=-0.05)and(y[i]<=0.05)):
            #   y[i]= 0.
    
        rms = np.sqrt(pow(y,2).mean())
        print "RMS:",rms
        if (rms > 0.04):
            plt.title("The hand is open :D")
        else:
            plt.title("The hand is closed D:")
        plt.plot(x, y)
        plt.plot(rms*level, c='r')
        plt.pause(1.0/60.0)
        

    except KeyboardInterrupt:
        plt.close()
        break

p.close()





