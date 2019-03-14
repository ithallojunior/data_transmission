/* Receiver
 *  It assumes the code sends the timing of something
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

// to select between the FAST (no ack) and SLOW trasmittion
//#define USE_FAST


#define CSN_PIN 7
#define CE_PIN 8
#define CHANNEL 76//0x4c
#define BUFFER_SIZE 4 //abs max per transmission 32Bytes, 16 for this
#define TOTAL_EXEC 1000 // maximum number of executions
#define BAUD 115200 // makes sure we can change easily the baudrate

RF24 radio(CE_PIN, CSN_PIN);


uint16_t data[BUFFER_SIZE];

const unsigned char pipe[5] = "TIMER";

unsigned int Max = 0; // for the data sent
unsigned int Min = 100000; // for the data sent

unsigned int Max_reception = 0; // for the time to acquire 
unsigned int Min_reception = 100000; // for the time to acquire

unsigned long start_;
unsigned long delta;

float total = 0;
float total_delta = 0;


void setup() {
  // serial settings
  Serial.begin(BAUD);
  delay(100);
  Serial.println("Starting");

  //radio settings
  radio.begin(); 
  radio.setChannel(CHANNEL);

   #ifdef USE_FAST
    radio.setRetries(0,0);
    radio.setAutoAck(false);
  #else
    radio.setRetries(15, 15); // max retries
    radio.setAutoAck(true);
  #endif

  //radio.setCRCLength(RF24_CRC_16);
  radio.setDataRate(RF24_2MBPS); //more than enough
  radio.setPayloadSize(sizeof(data)); // 2 x buffer size


  radio.openReadingPipe(1, pipe); 
  delay(100);
  radio.startListening();

  //the loop mus be here as it needs to do something and stop doing it
  uint16_t i = 0;
  while(i < TOTAL_EXEC){
   start_  = micros(); 
   if (radio.available()){
    radio.read( &data, sizeof(data));
    delta = micros() - start_;
    total_delta += delta;

    //ifs for the delta of the reception
    if (delta > Max_reception)
       Max_reception = delta;
    if (delta < Min_reception)
      Min_reception = delta;

      for (uint8_t j=0; j<BUFFER_SIZE;j++){
        
        // to get the mean
        total += data[j];

        //ifs to max and min
         if (data[j] > Max )
            Max = data[j];
          if(data[j] < Min )
            Min = data[j];
        }
    i++; //only increases if data were receiveid
   }
  
  }
  Serial.println("Finished");
  Serial.print("Transmitter time (us): ");
  Serial.print("Total: ");
  Serial.print(total);
  Serial.print("| Mean: ");
  Serial.print(total/(float)(TOTAL_EXEC * BUFFER_SIZE));
  Serial.print("| Max.: ");
  Serial.print(Max);
  Serial.print("| Min.: ");
  Serial.println(Min);


  Serial.print("Receiver time(us): ");
  Serial.print("Total: ");
  Serial.print(total_delta);
  Serial.print("| Mean: ");
  Serial.print(total_delta/(float)TOTAL_EXEC);
  Serial.print("| Max.: ");
  Serial.print(Max_reception);
  Serial.print("| Min.: ");
  Serial.println(Min_reception);
}


void loop(void){
  //yes, it is empty
  }
