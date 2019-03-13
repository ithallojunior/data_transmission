 /* Receiver
 * This code is for multiple channel trasmission and reception. In order for me
 * to differentiate the devices I marked them with white dots, so
 * they are called 1_DOT, 2_DOT, and so on. This is because of the 40 bit nature,
 * of the pipe name (5 Bytes). They  can just differ on the first byte.
 */
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

//#define DEBUG //uncomment fo debugging

// if both commented  or both not commented,  the multi device read is activated
//#define DOT_1 // activates the first device
#define DOT_2 // activate the second device



#ifdef DOT_1 && !(DOT_2)
#else 
  #ifdef DOT_2 && !(DOT_1)
   #else
      #define BOTH
    #endif
#endif




//RECEIVER
#ifdef DEBUG
#else
  #define TWOBYTES // uncomment to use the two bytes through USB sending protocol
#endif


#define CSN_PIN 7
#define CE_PIN 8
#define CHANNEL 76//0x4c
#define BUFFER_SIZE 4 //abs max per transmission 32Bytes, 16 for this

RF24 radio(CE_PIN, CSN_PIN);



//defining addresses
#ifdef DOT_1 && !(DOT_2)
  const unsigned char pipe[5] = "1_DOT";
  uint16_t data[BUFFER_SIZE];
#else 
  #ifdef DOT_2 && !(DOT_1)
    const unsigned char pipe[5] = "2_DOT";
    uint16_t data[BUFFER_SIZE];
   #else
      const unsigned char pipe[2][5] = {"1_DOT", "2_DOT"};
      //two buffers, as I'm reading two pipes
      uint16_t data_dot_1[BUFFER_SIZE];
      uint16_t data_dot_2[BUFFER_SIZE];
      bool new_dot_1 =  false;
      bool new_dot_2 = false;
      uint8_t pipeNum; 
    #endif
#endif

unsigned int value = 0;
int t1, t2;


// only valid  if one to one
#ifdef BOTH

  // only valid  if one to one
  void multi_device() {
  
      t1 = micros();
      
      new_dot_1 = false;
      new_dot_2 = false;
      
      //only exits if data available on both
      while( (!new_dot_1) && (!new_dot_2)) {
        
        if (radio.available(&pipeNum)) {
          
          if (pipeNum==1){
            radio.read(&data_dot_1, sizeof(data_dot_1) );
            new_dot_1 = true;
          }
          else{
            radio.read(&data_dot_2, sizeof(data_dot_2) );
            new_dot_2 = true;   
          }
        }
      }
      
       t2 = micros() - t1;

       for (int i=0;i<(BUFFER_SIZE);i++){
   
          #ifdef DEBUG
            // DOT 1
            Serial.print(" Value DOT_1: ");
            Serial.print(data_dot_1[i]);
            // DOT 2
            Serial.print("| Value DOT_2: ");
            Serial.print(data_dot_2[i]);
            //round time
            Serial.print(" ||Full pkg Time: ");
            Serial.print(t2);
            Serial.println("us");

       
          #else 
              #ifdef TWOBYTES
                // DOT 1
                Serial.write(data_dot_1[i]/256);
                Serial.write(data_dot_1[i]%256);
                // DOT 2
                Serial.write(data_dot_2[i]/256);
                Serial.write(data_dot_2[i]%256);
              #endif
          #endif
     }
           
  }

  
  


#else

  void single_device(){
  
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
  }

#endif




void setup() {
 
  Serial.begin(115200);
  delay(1000);
  
  #ifdef DEBUG
    Serial.print("\nStarting RX at pipe(s): ");
    #ifdef DOT_1 && !(DOT_2)
      Serial.println("DOT_1");
    #else
      #ifdef DOT_2 && !(DOT_1)
        Serial.println("DOT_2");
      #else
        Serial.println("DOT_1 DOT_2");
      #endif
    #endif
  #endif
    
  radio.begin(); 
  radio.setChannel(CHANNEL);
  radio.setAddressWidth(5);
  //radio.enableAckPayload();
  radio.setAutoAck(false);
  //radio.setCRCLength(RF24_CRC_16);
  radio.setDataRate(RF24_2MBPS); //more than enough
  radio.setPayloadSize(BUFFER_SIZE * sizeof(uint16_t)); // 2 x buffer size
  //radio.setRetries(0,15); 



  //opening reading pipes, I know it's ugly, but the other way around didn't work
  #ifdef BOTH
    radio.openReadingPipe(1, pipe[0]);
    delay(250);
    radio.openReadingPipe(2, pipe[1]);
    delay(250);
  #else
    radio.openReadingPipe(1, pipe);
    delay(250);
  #endif

  //start
  radio.startListening();
 
  
}


void loop(void){

   //sending data
    #ifdef BOTH
      multi_device();
    #else
      single_device();
    #endif
      
 
    }
 


