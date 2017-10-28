/*
 *  This codes reads the signal from the attiny and sends it
 *  to the the serial port, so that it can be plotted.
 */
#define CE_PIN 7
#define CSN_PIN 8
#define BYTES 20

#include <SPI.h>
#include "RF24.h"

RF24 radio(CE_PIN, CSN_PIN);

byte address[11] = "SimpleNode";
byte data[BYTES+1];

int value = 0;
void setup() {
  Serial.begin(230400);
  radio.begin(); // Start up the radio
  radio.setPayloadSize(BYTES+1);
  radio.setAutoAck(1); // Ensure autoACK is enabled
  radio.setRetries(15,15); // Max delay between retries & number of retries
  radio.openReadingPipe(1, address); // Write to device address 'SimpleNode'
  radio.startListening();
  delay(1000);
  
}


void loop(void){
  radio.read( &data, (BYTES+1)*sizeof(byte) );
  
    if  (data[0]=='f') {   
      for (int i=1;i<(BYTES);i+=2){
         value = (data[1]<<8) | (data[2]&255);
         Serial.write(value/256);
         Serial.write(value%256);
      }


  }

}
