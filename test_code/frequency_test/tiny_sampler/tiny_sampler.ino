//SENDER with self-regulating timer

#include "nRF24L01.h"
#include "RF24.h"


#define CSN_PIN 7//4
#define CE_PIN 8//5 //reset pin
#define CHANNEL 76//0x4c

#define BUFFER_SIZE 15 //max 255, real buffer size
#define ANALOG_PIN 3 //PB3, pin 2
#define T_SAMPLING 666 //usenconds, 2000Hz
#define DELTA T_SAMPLING - 144 // measured previously for exactly 500Hz 


uint16_t data[BUFFER_SIZE];
const unsigned char pipe[9] = "NRF24l01p";
//unsigned int payload = 0;
unsigned int start, delta; 
unsigned int i = 0;
unsigned int time_delta = DELTA;
bool just_sent = false;


RF24 radio(CE_PIN, CSN_PIN);

void setup() {
  
  radio.begin(); 
  radio.setChannel(CHANNEL);
  //radio.enableAckPayload();
  radio.setAutoAck(true);
  radio.setPayloadSize(sizeof(data)); // 2 x buffer size
  //radio.setCRCLength(RF24_CRC_16);
  radio.setRetries(1,2);
  radio.setDataRate(RF24_1MBPS); //more than enough
  
  radio.stopListening();
  radio.openWritingPipe(pipe);
  
  //analogReference(INTERNAL); // sets attiny85 reference voltage to 1v1
}

void loop(void){  
    
    // fill the buffer
    i = 0;
    while(i < BUFFER_SIZE){

     if(just_sent){
        delta = micros() - start; //TODO check for overflow
        if (delta>=DELTA){
        time_delta = (DELTA - delta); //just compensating for the sending time
        }
        else{
          time_delta = false;
        }
        just_sent = false;
      }
      else{
        time_delta  = DELTA;  
      }
      delayMicroseconds(time_delta);
      data[i] = analogRead(ANALOG_PIN);
      i += 1; 
      } 
   
 //send data 
 start = micros();
 just_sent = radio.write(&data, sizeof(data) );
 }
 

