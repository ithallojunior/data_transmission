// NOT GOOD, DONT USE IT


#define BUFFER 2 
#define T_SAMPLING 500 //useconds, 2000Hz
#define DELTA T_SAMPLING //12 // measured previously for exactly 500Hz 
// it oscillates between 1996.101 and 2000.000, but stays mostly in 2000.000



unsigned long int start, delta, start_sending, delta_sending; 
unsigned int i = 0;
unsigned long int time_delta = DELTA;
//bool just_sent = false;


bool state = true; 
unsigned int last_error = 0;
unsigned long last_us = 0L;


// function that accounts for all delays


unsigned int sampling_delay(unsigned int previous_error){
  
  unsigned int sampling_mean = 120; //us around, the error will be added later
  unsigned int max_error_drift = 30; //us around, max error time
  unsigned int error = 0; // us, adds one per round
  if (previous_error==max_error_drift)
    error = 0;
  else  
    error = previous_error + 1;

  delayMicroseconds(sampling_mean + error);
   
  return error;
}

unsigned int sending_delay(unsigned int previous_error){
  
  unsigned int sending_mean = 200; //us around, the error will be added later
  unsigned int max_error_drift = 30; //us around, max error time
  unsigned int error = 0; // us, adds one per round
  if (previous_error==max_error_drift)
    error = 0;
  else  
    error = previous_error + 1;

  delayMicroseconds(sending_mean + error);
   
  return error;
}


void setup() {
  pinMode(3, OUTPUT);

  //timers
  delta_sending = 0; 
  delta = 0;

}


void loop(void){  
    
    // fill the buffer
    for(i=0;i < BUFFER;i++){
      
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
      
      digitalWrite(3, state);
      state = !state;
      last_error = sampling_delay(last_error);
      
      while(delta < time_delta ){// my delta
        delta = micros() - start;
      }
      delta = 0;//zeroes the timer
      
      
      } 
   
 //send data 
 start_sending = micros();

 last_error = sending_delay(last_error);
 delta_sending = micros() - start_sending;
 
 }
 

