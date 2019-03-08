/*SENDER with self-regulating timer
 * This code is for multiple channel trasmission and reception. In order for me
 * to differentiate the devices I marked them with white dots, so
 * they are called 1_DOT, 2_DOT, and so on. This is because of the 40 bit nature
 * of the pipe name (5 Bytes). They  can just differ on the first byte.
 */


#include "nRF24L01.h"
#include "RF24.h"

//#define DOT_1 // if commented it activates the second device

#define CSN_PIN 4//7//4
#define CE_PIN 5//8//5 //reset pin
#define CHANNEL 76//0x4c

#define BUFFER_SIZE 4 //max 255, real buffer size
#define ANALOG_PIN 3 //PB3, pin 2
#define T_SAMPLING 500 //useconds, 2000Hz
#define DELTA T_SAMPLING - 18// error time


// setting the name of the transmitter
#ifdef DOT_1
  const unsigned char pipe[5] = "1_DOT";
#else
  const unsigned char pipe[5] = "2_DOT";
#endif

uint16_t data[BUFFER_SIZE];
unsigned long int start, delta, start_sending, delta_sending; 
unsigned int i = 0;
unsigned long int time_delta = DELTA;


RF24 radio(CE_PIN, CSN_PIN);

void setup() {
  
  radio.begin(); 
  radio.setChannel(CHANNEL);
  radio.setAddressWidth(5);

  radio.setAutoAck(false);
  radio.setPayloadSize(sizeof(data)); // 2 x buffer size
  radio.setRetries(0,0);
  radio.setDataRate(RF24_2MBPS); //more than enough
  
  radio.stopListening();
  radio.openWritingPipe(pipe);
  
  analogReference(INTERNAL); // sets attiny85 reference voltage to 1v1

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
 

