/*
A simple oscillator with 50% cycle to the attiny85
that will be used to calibrate its internal oscillator.
It writes the callibration factor to the eeprom starting
at 0.
 */
#include <EEPROM.h>

#define PIN 3
#define HALF_CYCLE 500 //useconds, 1kHz
#define CLOCK_ADJUST_FOR_THIS_CHIP -6

//#define LOAD_FROM_EEPROM // enables the loading from the eeprom
//#define WRITE_TO_EEPROM //enables writing to eeprom

unsigned long int t=0;
bool initial_state=0;


void setup() {
  //Serial.begin(115200);
  //delay(1000);
  int value=CLOCK_ADJUST_FOR_THIS_CHIP;
  
  #ifdef WRITE_TO_EEPROM
    char *p;
    p = (char *) &value;
    for (unsigned int i=0;i<sizeof(value); i++){
      EEPROM.update(i, *p++);
    }
    //Serial.println("wrote to eeprom");
  #endif
  
  #ifdef LOAD_FROM_EEPROM
   EEPROM.get(0, value);
  //Serial.println("read from eeprom");
  //Serial.println(value);
  #endif

  // calibration for this specific IC 

 //Serial.print("value: ");
  //Serial.println(value);

  //Serial.print("OSCCAL: ");
  //Serial.println(OSCCAL);


  OSCCAL += value;

  //Serial.print("OSCCAL AFTER: ");
  //Serial.println(OSCCAL);
  
  pinMode(PIN, OUTPUT);
}


void loop(void){  
  
  initial_state ^= 1;
  digitalWrite(PIN, initial_state);
  
  while( (micros() - t) <= HALF_CYCLE);
  t += HALF_CYCLE;
  
    }
