 /* 
 * Receiver
 */
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

//#define DOT_1 // activates the first device
//#define DOT_2 // activate the second device

#define CSN_PIN 7
#define CE_PIN 8
#define CHANNEL 76//0x4c
#define BUFFER_SIZE 1 //abs max per transmission 32Bytes, 16 for this
#define DEVICES 1
const unsigned char pipes[2][5] = {"2SEMG", "1SEMG"};

RF24 radio(CE_PIN, CSN_PIN);


uint16_t buffer[DEVICES][BUFFER_SIZE];

uint8_t pipe = 0;
uint8_t previous_pipe = 0;
uint8_t pipe_count = 0;

void receiver() {

  pipe_count = 0;
  previous_pipe = DEVICES + 1; // in order to be different for only one device
  while(pipe_count < DEVICES){
    
    if (radio.available(&pipe)){

      radio.read(&buffer[pipe-1], sizeof(buffer[0]));
      
      if (pipe != previous_pipe){

        pipe_count += 1;
        previous_pipe = pipe;
      }

    }
  
  }
  
  for (int i=0;i<BUFFER_SIZE;i++){  
    
    for (int j=0; j<DEVICES; j++){
    
      Serial.write(buffer[j][i]/256);
      Serial.write(buffer[j][i]%256);
    }
  
  }

}


void setup() {
 
  Serial.begin(115200);
  delay(1000);
    
  radio.begin(); 
  radio.setChannel(CHANNEL);
  radio.setAddressWidth(5);
  //radio.enableAckPayload();
  
  radio.setAutoAck(false);
  radio.setRetries(0,0); 
  //radio.setCRCLength(RF24_CRC_16);
  radio.setDataRate(RF24_2MBPS); //more than enough
  radio.setPayloadSize(BUFFER_SIZE * sizeof(uint16_t)); // 2 x buffer size
  

  for (int i=1; i<=DEVICES; i++){
  
    radio.openReadingPipe(i, pipes[i-1]);
    delay(250);
  }

  //start
  radio.startListening();
 
  
}


void loop(void){

  receiver();
}
 


