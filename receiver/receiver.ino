#define CE_PIN 7
#define CSN_PIN 8

#include <SPI.h>
#include "RF24.h"

RF24 radio(CE_PIN, CSN_PIN);

byte address[11] = "SimpleNode";
int payload = 0;

void setup() {
  Serial.begin(115200);
  radio.begin(); // Start up the radio
  radio.setAutoAck(1); // Ensure autoACK is enabled
  radio.setRetries(15,15); // Max delay between retries & number of retries
  radio.openReadingPipe(1, address); // Write to device address 'SimpleNode'
  radio.startListening();
}

void loop(void){
  radio.stopListening();
  radio.startListening();
  radio.read( &payload, sizeof(int) );
   //radio.read( &payload, sizeof(char) );
  if(payload != 0){
    Serial.print("Got Payload ");
    Serial.println(payload);
    
    //Serial.print("Data rate:");
    //Serial.println(radio.getDataRate());
  }
  delay(1000);
}
