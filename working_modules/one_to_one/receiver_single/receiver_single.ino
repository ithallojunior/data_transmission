#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
//RECEIVER
//#define DEBUG //uncomment fo debugging
#ifdef DEBUG
#else
  #define TWOBYTES // uncomment to use the two bytes through USB sending protocol
#endif

#define CSN_PIN 7
#define CE_PIN 8
#define CHANNEL 76//0x4c
#define BUFFER_SIZE 4 //abs max per transmission 32Bytes, 16 for this

RF24 radio(CE_PIN, CSN_PIN);


uint16_t data[BUFFER_SIZE];

const unsigned char pipe[9] = "NRF24l01p";
//byte data[BUFFER_SIZE];
unsigned int value = 0;
int t1, t2;

void setup() {
 
  Serial.begin(115200);
  delay(1000);
  #ifdef DEBUG
    Serial.println("Starting RX");
  #endif 
  radio.begin(); 
  radio.setChannel(CHANNEL);
  //radio.enableAckPayload();
  radio.setAutoAck(false);
  //radio.setCRCLength(RF24_CRC_16);
  radio.setDataRate(RF24_2MBPS); //more than enough
  radio.setPayloadSize(sizeof(data)); // 2 x buffer size
  //radio.setRetries(0,15); 

  
  radio.openReadingPipe(1, pipe); 
  delay(1000);
  radio.startListening();
 
  
}

void loop(void){
   t1 = micros();
    if (radio.available()) {
      
      radio.read( &data, sizeof(data) );
      t2 = micros() - t1;
      
      //Serial.println(data[0]);
      
      for (int i=0;i<(BUFFER_SIZE);i++){
        
          value = data[i];//(data.a[i]<<8) | (data.b[i]&255);
          #ifdef DEBUG
            Serial.print("Value: ");
            Serial.print(value);
            Serial.print(" || Pkg Time: ");
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
    //#ifdef DEBUG
     //  Serial.println("No data ");
    //#endif
    }
  }


