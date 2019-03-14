/*SENDER with simpler timer, for some reason I don't know yet this one
 * produced the clearer signal.
 * This code is for multiple channel trasmission and reception. In order for me
 * to differentiate the devices I marked them with white dots, so
 * they are called 1_DOT, 2_DOT, and so on. This is because of the 40 bit nature
 * of the pipe name (5 Bytes). They  can just differ on the first byte.
 */
#include <nRF24L01.h>
#include <RF24.h>


/* Select the device here  by commenting or uncommenting the next line.*/
//#define DEVICE_1 


#define CSN_PIN 4//7//4
#define CE_PIN 5//8//5 //reset pin
#define CHANNEL 76
#define BUFFER_SIZE 4 //max 255, real buffer size
#define ANALOG_PIN 3 //PB3, pin 2
#define REFERENCE_TIME 490 //useconds, because reasons, it was made to be 2040 Hz

/*  
 * The expected sampling frequency is around 2040 Hz, adjusted here 
 * because of clock errors
*/

#ifdef DEVICE_1
  #define DELTA REFERENCE_TIME + 15 // the 500 us results in 2060 on this attiny, so the correction
  const unsigned char pipe[5] = "1_DOT";
#else
  #define DELTA REFERENCE_TIME + 11 
  const unsigned char pipe[5] = "2_DOT";
#endif
  
uint16_t data[BUFFER_SIZE];
uint8_t i = 0;
unsigned long last_us = 0L;

RF24 radio(CE_PIN, CSN_PIN);
void setup(){
  // radio  settings
  radio.begin(); 
  radio.setChannel(CHANNEL);
  radio.setAddressWidth(5);
  
  radio.setAutoAck(false);
  radio.setRetries(0,0);
  
  radio.setPayloadSize(sizeof(data)); // 2 x buffer size
  radio.setDataRate(RF24_2MBPS); //more than enough
  
  radio.stopListening();
  radio.openWritingPipe(pipe);
  //end radio settings
  
  // sets attiny85 reference voltage to 1v1
  analogReference(INTERNAL); 
}


void loop(){  
  i = 0;
  while(i < BUFFER_SIZE){
    if (micros() - last_us > DELTA){
      last_us += DELTA;
      data[i] = analogRead(ANALOG_PIN);
      i++;
     }
  }
  
//send data 
//just_sent = radio.write(&data, sizeof(data) );
  radio.writeFast(&data, sizeof(data) );
  
}
