//SENDER with self-regulating timer

#include "nRF24L01.h"
#include "RF24.h" 


//#define TIME_DEBUG // to send the delta time instead of the value
 
#define CSN_PIN 4//7//4
#define CE_PIN 5//8//5 //reset pin
#define CHANNEL 76//0x4c

#define BUFFER_SIZE 4 //real buffer size
#define ANALOG_PIN 3 //PB3, pin 2
#define T_SAMPLING 500 //useconds, 2000Hz
//#define DELTA T_SAMPLING - 18//12 // measured previously for exactly 500Hz 


const unsigned char pipe[5] = "1_DOT";
uint16_t data[BUFFER_SIZE];
uint8_t i = 0;
unsigned long last_us = 0L;

#ifdef TIME_DEBUG
  uint16_t start_;
#endif



RF24 radio(CE_PIN, CSN_PIN);

void setup() {
  
  radio.begin(); 
  radio.setChannel(CHANNEL);
  //radio.enableAckPayload();
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
    i = 0;
    
    while(i< BUFFER_SIZE){
     
    if (micros() - last_us > T_SAMPLING){
      last_us += T_SAMPLING;
      #ifdef TIME_DEBUG
        start_ = micros();
        data[i] = analogRead(ANALOG_PIN);
        data[i] = micros() - start_;
       #else
        data[i] =  analogRead(ANALOG_PIN);
       #endif
      i++;
    }
      
      } 
     
   
 //send data 
 //start_ = micros();
 //just_sent = radio.write(&data, sizeof(data) );
 radio.writeFast(&data, sizeof(data) );
 //delta = micros() - start_;

 }
 

