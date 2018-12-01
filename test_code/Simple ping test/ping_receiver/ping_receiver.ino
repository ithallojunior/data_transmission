#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <printf.h>
//RECEIVER

//defining it here makes it easier to use with other pins
#define CSN_PIN 7
#define CE_PIN 8

#define CHANNEL 0x4c
#define  BUFFER_SIZE 32

RF24 radio(CE_PIN, CSN_PIN);


//const uint64_t pipe =  0xABCDABCD71LL;
const byte pipe[9] = "NRF24l01p";
const char dummyText[] = "ack"; 
char text[BUFFER_SIZE]; //"012345678901234567891234567890123";

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 1);
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting receiver");
  
  radio.begin();
  //some configs
  //radio.setPALevel(RF24_PA_MIN);//TODO change to min
  radio.enableAckPayload();
  radio.enableDynamicPayloads();
  radio.setChannel(CHANNEL);
  radio.setRetries(0, 15); // sender
  printf_begin();
  radio.printDetails();
  
  Serial.print("Is it connected? ");
  Serial.println(radio.isChipConnected());
  //radio.setDataRate(RF24_250KBPS);
  //radio.setCRCLength(RF24_CRC_8);
  //radio.setPayloadSize(32);// amx
  //end configs

  radio.openReadingPipe(1, pipe);
  delay(1000);
  radio.startListening();
 
}




void loop() {
  if (radio.available()) {
    radio.read( &text, sizeof(text));
    String transData = String(text);
    if (transData == dummyText) {
        Serial.println("ACK RECEIVED");
        digitalWrite(LED_BUILTIN, 0); 
        delay(500);     
      }    
    else{
        Serial.print("NO ACK | GARBAGE RECEIVED: ");
        Serial.println(transData);
        digitalWrite(LED_BUILTIN, 1);
        }
      }

   digitalWrite(LED_BUILTIN, 1);

}
  


