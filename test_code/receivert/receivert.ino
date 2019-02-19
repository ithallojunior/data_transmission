#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
//RECEIVER

//#define DEBUG //uncomment fo debugging
//#define TWOBYTES // uncomment to use the two bytes through USB sending protocol

#define CSN_PIN 7
#define CE_PIN 8
#define CHANNEL 0x4c
#define BUFFER_SIZE 20

RF24 radio(CE_PIN, CSN_PIN);

const byte pipe[9] = "NRF24l01p";
byte data[BUFFER_SIZE];
int value = 0;
int t1, t2;

void setup() {
 
  Serial.begin(115200);
  delay(1000);
  #ifdef DEBUG
    Serial.println("Starting RX");
  #endif 
  radio.begin(); // Start up the radio
  radio.setChannel(CHANNEL);
  radio.enableAckPayload();
  radio.setPayloadSize(BUFFER_SIZE);
  radio.setRetries(0,15); 
  
  radio.openReadingPipe(1, pipe); 
  delay(1000);
  radio.startListening();
 
  
}

void loop(void){
  t1 = micros();
  
    if (radio.available()) {
      radio.read( &data, (BUFFER_SIZE)*sizeof(byte) );
      t2 = micros() - t1;
      
      //Serial.println(data[0]);
      
      for (int i=0;i<(BUFFER_SIZE);i+=2){
        
          value = (data[i]<<8) | (data[i+1]&255);
          #ifdef DEBUG
            Serial.print("Value: ");
            Serial.print(value);
            Serial.print(" || Time: ");
            Serial.print(t2);
            Serial.println("us");
          #else 
              #ifdef TWOBYTES
                Serial.write(value/256);
                Serial.write(value%256);
              #else
                Serial.println((value/1023.)*1.1);
              #endif
          #endif
        }  

    }
  else{
    #ifdef DEBUG
       Serial.print("No data ");
    #endif
    }
  }


