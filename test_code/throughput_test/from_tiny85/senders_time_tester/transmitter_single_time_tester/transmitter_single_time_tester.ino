//SENDER with self-regulating timer

#include "nRF24L01.h"
#include "RF24.h" 



// to differ between the transmission time and sampling time
//#define TRANSMISSION_DEBUG 

// to select between the FAST (no ack) and SLOW trasmittion
//#define USE_FAST



 
#define CSN_PIN 4//7//4
#define CE_PIN 5//8//5 //reset pin
#define CHANNEL 76

#define BUFFER_SIZE 4 //real buffer size
#define ANALOG_PIN 3 //PB3, pin 2
#define T_SAMPLING 500 //useconds, does not realy matter for the case


const unsigned char pipe[5] = "TIMER";
uint16_t data[BUFFER_SIZE];
uint8_t i = 0;
unsigned long last_us = 0L;
uint16_t start_ = 0;
uint16_t delta = 0;
uint16_t delta_transmission = 0;



RF24 radio(CE_PIN, CSN_PIN);

void setup() {
  //radio settings
  radio.begin(); 
  radio.setChannel(CHANNEL);
  //radio.enableAckPayload();
  radio.setPayloadSize(sizeof(data)); // 2 x buffer size
  radio.setDataRate(RF24_2MBPS); //more than enough

  #ifdef USE_FAST
    radio.setRetries(0,0);
    radio.setAutoAck(false);
  #else
    radio.setRetries(15, 15); // max retries
    radio.setAutoAck(true);
  #endif
  
  radio.stopListening();
  radio.openWritingPipe(pipe);

  //setting internal ref to 1v1
  analogReference(INTERNAL); // sets attiny85 reference voltage to 1v1

}


void loop(void){  
    
    // fill the buffer
    i = 0;
    while(i< BUFFER_SIZE){
     
      if (micros() - last_us > T_SAMPLING){
        last_us += T_SAMPLING;
        
        #ifdef TRASMISSION_DEBUG
          data[i] = delta_transmission;
        #else
          start_ = micros();
          data[i] = analogRead(ANALOG_PIN);
          delta = micros() - start_;
          data[i] = delta;
        #endif
        i++;
      }
      
    } 
     
   
 //send data 

  #ifdef TRANSMISSION_DEBUG
    start_ = micros();
  #endif
  
  #ifdef USE_FAST
    radio.writeFast(&data, sizeof(data) );
  #else
    radio.write(&data, sizeof(data) );
  #endif

   #ifdef TRANSMISSION_DEBUG
    delta_transmission = micros() - start_;
   #endif

 }
