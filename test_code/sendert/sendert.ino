#define ATTINY85 //uncomment for attiny85
#ifdef ATTINY85
#else
  #include <SPI.h> //commented for attiny85
#endif
#include "nRF24L01.h"
#include "RF24.h"
//SENDER

//#define DEBUG //uncomment for debug
#ifdef ATTINY85
  #define CSN_PIN 4
  #define CE_PIN 5//reset pin
#else
  #define CSN_PIN 7
  #define CE_PIN 5
#endif
#define BUFFER_SIZE 10 //max 255
#define CHANNEL 0x4c
#define MAX_DELAY  100 //useconds
#define T_SAMPLING 500 //usenconds, 2kHz
#define ANALOG_PIN 3

//byte data[BUFFER_SIZE];
const byte pipe[9] = "NRF24l01p";
int payload = 0;
unsigned long start, delta;

const unsigned long time_delta = T_SAMPLING - MAX_DELAY;

struct dataBuffer{
    unsigned char a[BUFFER_SIZE];
    unsigned char b[BUFFER_SIZE];
};

struct dataBuffer data;

int toggle = 1;
int i = 0;
bool sent;


RF24 radio(CE_PIN, CSN_PIN);

void setup() {
  #ifdef DEBUG
    Serial.begin(115200);
    delay(1000);
    Serial.println("Starting TX");
  #endif
  radio.begin(); 
  radio.setChannel(CHANNEL);
  radio.enableAckPayload();
  radio.setPayloadSize(sizeof(data)); 
  radio.setRetries(0,15);

  radio.stopListening();
  radio.openWritingPipe(pipe);
  
  #ifdef ATTINY85
    analogReference(INTERNAL);
  #endif
  delay(1000);
  start = micros();
}

void loop(void){

    while(i < (BUFFER_SIZE)){

      payload = analogRead(ANALOG_PIN);

      delta = micros() - start;
      if(delta >= time_delta){
        data.a[i] = (payload>>8);
        data.b[i] = (payload&255);
        i += 1; 
        #ifdef DEBUG
          Serial.print("Payload: ");
          Serial.print(payload);
          Serial.print("Time delta: ");
          Serial.println(delta);
        #endif
        start = micros();
        }
    } 
 sent = radio.write( &data, sizeof(data) );
 if (sent){
  #ifdef DEBUG
   Serial.println("Data sent");
  #endif
  i = 0;
  start = micros(); // assings again to remove sending time
  }
 }
 

