#define CE_PIN 5 //reset pin
#define CSN_PIN 4 //Since we are using 3 pin configuration we will use same pin for both CE and CSN
#define BYTES 20 //max 255

#include "RF24.h"

RF24 radio(CE_PIN, CSN_PIN);
byte data[BYTES+1]; // +1 beacause of the ack
byte address[11] = "SimpleNode";
int payload = 0;

void setup() {
  
  radio.begin(); // Start up the radio
  radio.setAutoAck(1); // Ensure autoACK is enabled
  radio.setRetries(15,15); // Max delay between retries & number of retries
  radio.openWritingPipe(address); // Write to device address 'SimpleNode'
  radio.setPayloadSize(BYTES+1); // +1 beacause of the ack
  analogReference(INTERNAL);
  delay(1000);
 
  data[0] = 'f'; // the ack
}

void loop(void){

  for (int i=1; i<(BYTES); i+=2){
  payload = analogRead(3);
  
  data[1] = (payload>>8);
  data[2] = (payload&255);
  delayMicroseconds(500); //delay for 2khz
 
  radio.write( &data, (BYTES+1)*sizeof(byte) ); //Send data to 'Receiver' ever second
 } // in case of error nothing is sent

}
