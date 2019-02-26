//SENDER with self-regulating timer

#include "nRF24L01.h"
#include "RF24.h"


#define CSN_PIN 7//4
#define CE_PIN 8//5 //reset pin
#define CHANNEL 76//0x4c

#define BUFFER_SIZE 4 //max 255, real buffer size
#define ANALOG_PIN 3 //PB3, pin 2
#define T_SAMPLING 498 //usenconds,around 2008 Hz
#define DELTA T_SAMPLING - 12//124 // measured previously for exactly 500Hz 
// after the adjusts it samples at 2007.843 Hz

const unsigned char pipe[9] = "NRF24l01p";
uint16_t data[BUFFER_SIZE];
unsigned long int start, delta, start_sending, delta_sending; 
unsigned int i = 0;
unsigned long int time_delta = DELTA;
//bool just_sent = false;

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
  
  //analogReference(INTERNAL); // sets attiny85 reference voltage to 1v1


  //timers
  delta_sending = 0; 
  delta = 0;

  //Serial.begin(115200);
  //Serial.println("Starting TX");
}


void loop(void){  
    
    // fill the buffer
    for(i=0;i < BUFFER_SIZE;i++){
      
      
      start = micros();

      if (i==0){
        if(delta_sending >= DELTA){
          time_delta = DELTA;
        }
        else{
          time_delta = DELTA -  delta_sending;
        }      
      }
      else{
        time_delta = DELTA;
      }

      data[i] = analogRead(ANALOG_PIN);
      while(delta < time_delta ){// my delta
        delta = micros() - start;
      }
      //Serial.print("delta ");
      //Serial.println(delta);
      //Serial.println(time_delta);
      delta = 0;//zeroes the timer
      
      //delayMicroseconds(time_delta);
      
      //data[i] = delta;
      } 
   
 //send data 
 start_sending = micros();
 //just_sent = radio.write(&data, sizeof(data) );
 radio.writeFast(&data, sizeof(data) );
 delta_sending = micros() - start_sending;
 
 }
 

