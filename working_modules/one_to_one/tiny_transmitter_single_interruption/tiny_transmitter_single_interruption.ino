//SENDER with interrupt

#include <nRF24L01.h>
#include <RF24.h>
// must be downloaded from https://github.com/PaulStoffregen/TimerOne
#include <TimerOne.h> 

#define CSN_PIN 4//7//4
#define CE_PIN 5//8//5 //reset pin
#define CHANNEL 76//0x4c

#define BUFFER_SIZE 4 //max 255, real buffer size
#define ANALOG_PIN 3 //PB3, pin 2
#define T_SAMPLING 500 //useconds, 2000Hz
#define DELTA T_SAMPLING //- 18//12 // measured previously for exactly 500Hz 



//all variables
const unsigned char pipe[5] = "1_DOT";
uint16_t data[BUFFER_SIZE];
unsigned int i = 0;
bool data_to_send_buffer_full = false;

//all funcions
void sampler(){
  if (i < BUFFER_SIZE){
    data[i] = analogRead(ANALOG_PIN);
    i++;
  }
  else{
    data_to_send_buffer_full = true;
  }
}


// must be declared before the function
RF24 radio(CE_PIN, CSN_PIN);
void radioConfig(){
   radio.begin(); 
  radio.setChannel(CHANNEL);
  //radio.enableAckPayload();
  radio.setAutoAck(false);
  radio.setPayloadSize(sizeof(data)); // 2 x buffer size
  radio.setRetries(0,0);
  radio.setDataRate(RF24_2MBPS); //more than enough
  
  radio.stopListening();
  radio.openWritingPipe(pipe); 
}


void setup() {
  analogReference(INTERNAL); // sets attiny85 reference voltage to 1v1
  radioConfig();
  Timer1.initialize(DELTA);
  Timer1.attachInterrupt(sampler);
  
}


void loop(void){  
    
   // all via interruption
  if(data_to_send_buffer_full){ 
  //just_sent = radio.write(&data, sizeof(data) );
  radio.writeFast(&data, sizeof(data) );
  data_to_send_buffer_full = false;//disarms the flag
  i = 0; //only starts the buffer again after sending data
  }
 
 }
 

