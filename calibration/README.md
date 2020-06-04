# Calibration

This part includes codes related to calibration of the internal oscillator
of the ATtiny85. Ideally either an oscilloscope or a frequency meter should be
used. 
In the lack of any of those, the paired codes *simple_sampler_nano.ino* 
(which samples at 4kHz) and the *python_code/attiny85_callibrator_helper.py* or
might help. The latter one is preferred over the *python_code/spectrum.py* code
due to its versatility.
The idea in here is that OSCCAL get adjusted and then its value get saved to
the EEPROM of the IC and then reloaded whenever it is needed.  

Regarding usage, the *attiny85_manual_calibrator.ino* was intended to be used 
with just one arduino and the ATtiny85 and the *attiny85_serial_calibrator.ino* 
needs at least two extra arduino boards (but it is way simpler).


# EEPROM

As found here, the EEPROM would get erased on reprogramming the IC,  so,
a fuse must be adjusted to prevent so.

The fuse settings are: **-U lfuse:w:0xe2:m -U hfuse:w:0xf7:m -U efuse:w:0xff:m**,
they must be added to you avrdude terminal command (copy it from the arduino 
interface).
Check https://www.engbedded.com/fusecalc/ for more information.




