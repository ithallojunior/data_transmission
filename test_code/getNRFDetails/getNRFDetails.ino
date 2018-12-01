#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h>

#define CHANNEL 0x4c
#define CSN_PIN 7
#define CE_PIN 8



RF24 radio(CE_PIN, CSN_PIN);

void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200);
 delay(1000);
 printf_begin();
  
 Serial.println("\n\n#########BEGIN#########");
 radio.begin();
  
  //some configs
  //radio.setPALevel(RF24_PA_MIN);//TODO change to min
  radio.enableAckPayload();
  radio.enableDynamicPayloads();
  radio.setChannel(CHANNEL);
  radio.setRetries(0, 15); // sender
  printf_begin();
  radio.printDetails();
  //end configs

  
  // Get into standby mode
  radio.startListening();
 //radio.stopListening();
  //check if it's properly connected before getting details
  bool isItConnected;
  isItConnected = radio.isChipConnected() ;
  if (isItConnected){
     Serial.println("It's connected :)");
  }
  else{
    Serial.println("It's  not connected :(");
  }
  radio.printDetails();
  Serial.println("###########DONE############");
}

void loop() {
//radio.printDetails();
//Serial.println("###########DONE############");
//delay(5000);
}
