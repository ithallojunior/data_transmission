#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
//SENDER

//defining it here makes it easier to use with other pins
#define CSN_PIN 7
#define CE_PIN 8
#define CHANNEL 0x4c


RF24 radio(CE_PIN, CSN_PIN);

//const uint64_t pipe =  0xABCDABCD71LL;
const byte pipe[9] = "NRF24l01p";
const char text[] = "ack";

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting sender");
  radio.begin();
  //some configs
  //radio.setPALevel(RF24_PA_MIN);//TODO change to min
  radio.enableAckPayload();
  radio.enableDynamicPayloads();
  radio.setChannel(CHANNEL);
  radio.setRetries(0, 15);
  printf_begin(); 
  radio.printDetails();
  Serial.print("Is it connected? ");
  Serial.println(radio.isChipConnected());
  //radio.setDataRate(RF24_250KBPS);
  //radio.setCRCLength(RF24_CRC_8);
  //radio.setPayloadSize(32);//max
  //end configs

  radio.stopListening();
  radio.openWritingPipe(pipe);
}




void loop() {
  bool sentIt; //True if the payload was delivered successfully false if not
  sentIt = radio.write(&text, sizeof(text));
  // works to see if it's workong or not
  if (sentIt){ 
    Serial.println("ACK TX OK");
  }
  else{
   Serial.println("TX NOT SENT "); 
  }
  delay(1000);
}




