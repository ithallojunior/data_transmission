/*
A simple oscillator with 50% cycle to the attiny85
that will be used to calibrate its internal oscillator.
It writes the callibration factor to the eeprom starting
at 0.
 */
#include <EEPROM.h>

#define PIN 0
#define HALF_CYCLE 500 //useconds, 1kHz
#define ADJUSTED_OSCCAL 82

/*
The following consts define whether it should load the value
from EEPROM or write to it, comment both to use the default one.
Remember that to keep the EEPROM data after reprogramming fuses must 
be set.
The fuses are:
-U lfuse:w:0xe2:m -U hfuse:w:0xf7:m -U efuse:w:0xff:m
*/
#define LOAD_FROM_EEPROM 
//#define WRITE_TO_EEPROM

unsigned long int t=0;
bool initial_state=0;
char value;

void setup() {
  
  #ifdef WRITE_TO_EEPROM
    EEPROM.update(0, (char) ADJUSTED_OSCCAL)
  #endif
  
  #ifdef LOAD_FROM_EEPROM
    EEPROM.get(0, value);
    OSCCAL = value;
  #endif
  
  pinMode(PIN, OUTPUT);
}


void loop(void){  
  
  initial_state ^= 1;
  digitalWrite(PIN, initial_state);
  
  while( (micros() - t) <= HALF_CYCLE);
  t += HALF_CYCLE;
  
    }
