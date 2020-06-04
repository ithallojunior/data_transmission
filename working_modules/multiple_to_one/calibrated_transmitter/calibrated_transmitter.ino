/*
SENDER which uses the pre-calibrated OSCCAL from EEPROM.

This code is for multiple channel transmission and reception. In order
to differentiate the devices they are marked with white dots, so
they are called 1_DOT, 2_DOT, and so on. This is because of the 40 bit nature
of the pipe name (5 Bytes). They  can just differ on the first byte.
*/

#include <EEPROM.h>
#include "nRF24L01.h"
#include "RF24.h"

//#define DOT_1 // if commented it activates the second device

#define CSN_PIN 4//7//4
#define CE_PIN 5//8//5 //reset pin
#define CHANNEL 76//0x4c

#define BUFFER_SIZE 4 //max 255, real buffer size
#define ANALOG_PIN 3 //PB3, pin 2
#define DELTA 500 //useconds, 2000Hz


// setting the name of the transmitter
#ifdef DOT_1
  const unsigned char pipe[5] = "1_DOT";
#else
  const unsigned char pipe[5] = "2_DOT";
#endif

uint16_t data[BUFFER_SIZE]; 
unsigned int i=0;
unsigned long int t=0;
unsigned char value;

RF24 radio(CE_PIN, CSN_PIN);

void setup() {

  //loading calibrated OSCCAL
  EEPROM.get(0, value);
  OSCCAL = value;

  radio.begin(); 
  radio.setChannel(CHANNEL);
  radio.setAddressWidth(5);

  radio.setAutoAck(false);
  radio.setPayloadSize(sizeof(data)); // 2 x buffer size
  radio.setRetries(0,0);
  radio.setDataRate(RF24_2MBPS); //more than enough
  
  radio.stopListening();
  radio.openWritingPipe(pipe);
  
  analogReference(INTERNAL); // sets attiny85 reference voltage to 1v1

}


void loop(void){  
    
    // fill the buffer
    for(i=0;i < BUFFER_SIZE;i++){ 

      data[i] = analogRead(ANALOG_PIN);
      
      while( (micros() - t) <= DELTA);
      t += DELTA;

    }
   
 //send data 
 radio.writeFast(&data, sizeof(data) );

 }
 
