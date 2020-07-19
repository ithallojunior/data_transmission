/*
A simple oscillator with 50% cycle to the attiny85
that will be used to calibrate its internal oscillator.
It writes the callibration factor to the eeprom starting
at 0.
 */


#define PIN 8
#define HALF_CYCLE 2000 //useconds

unsigned long int t=0;
bool initial_state=0;


void setup() { 
  pinMode(PIN, OUTPUT);
  
}


void loop(void){  
  
  initial_state ^= 1;
  digitalWrite(PIN, initial_state);
  
  while( (micros() - t) <= HALF_CYCLE);
  t += HALF_CYCLE;
  
    }
