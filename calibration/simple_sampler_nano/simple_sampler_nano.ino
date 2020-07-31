/*
A simple sampler based on wait and sample. Creates
a buffer to make it easier.
 */

#define BUFFER_SIZE 4
#define ANALOG_PIN 3 
#define SAMPLING_FREQUENCY 4000 //HZ
#define DELTA 1000000/SAMPLING_FREQUENCY  //useconds

uint16_t buffer[BUFFER_SIZE];
unsigned long int now=0, t=0;
//unsigned long int sending_start=0, sending_delta=0;
//unsigned long int t0=0, t1=0;
unsigned int i = 0;

void setup() {

  Serial.begin(115200);
  //Serial.println("Starting TX");
}


void loop(void){  
    
    for(i=0;i < BUFFER_SIZE;i++){
      //t0 = micros();
       
      while( (micros() - t) <= DELTA);
      t += DELTA;

      buffer[i] = analogRead(ANALOG_PIN);
      
      //t1 = micros() - t0; 
    }
  
  // sending
  //sending_start = micros();
  
  for(i=0;i < BUFFER_SIZE;i++){
    //Serial.println( 5*(buffer[i]/1023.)); 

    Serial.write(buffer[i]/256);
    Serial.write(buffer[i]%256);   
  
  } 

  // timing
  //Serial.print("time(us): ");
  //Serial.println(t1);
  //Serial.println(sending_delta);

  //sending_delta = micros() - sending_start;
 }
 

