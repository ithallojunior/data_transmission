#define CE_PIN 5 //reset pin
#define CSN_PIN 4 //Since we are using 3 pin configuration we will use same pin for both CE and CSN

#include "RF24.h"

RF24 radio(CE_PIN, CSN_PIN);
//char my_ack = "k"; 
byte address[11] = "SimpleNode";
int payload = 0;

void setup() {
  radio.begin(); // Start up the radio
  radio.setAutoAck(1); // Ensure autoACK is enabled
  radio.setRetries(15,15); // Max delay between retries & number of retries
  radio.openWritingPipe(address); // Write to device address 'SimpleNode'
  analogReference(INTERNAL);
}

void loop(void){

  //payload = payload+3;
   payload = analogRead(3);
  //delay(10);
  radio.write( &payload, sizeof(int) ); //Send data to 'Receiver' ever second
  //radio.write( &my_ack, sizeof(char) );
  delay(1000);
}
