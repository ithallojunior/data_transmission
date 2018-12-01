import serial
import numpy as np
import time
import matplotlib.pyplot as plt
pd = "/dev/tty.usbmodem3a21"

p = serial.Serial(port=pd, baudrate=230400,
                  bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE)
p.flush()
samples = 2000
acc = 0.0
y = []
total = 15 #runs

for j in xrange(0, total):
	i = 0
	print ("#run # %s"%j)
	while i < samples:
		start = time.time()
		value1 = p.read()
		value2 = p.read()
		v = ord(value1[0])*256 + ord(value2[0])
		t = time.time() - start
		
		i = i +1
		y.append(t)	
		acc += t
		#print t

print ("\n\nTOTAL TIME: %s seconds of %s intended"%(acc, total))
print ("AVG TIME: %f seconds"%(acc/(samples*total)))

#plt.plot(y)
#plt.ylabel("time(s)")
#plt.xlabel("sample")
#plt.show()
p.close()

