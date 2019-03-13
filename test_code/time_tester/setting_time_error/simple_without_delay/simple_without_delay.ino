// THAT'S OK, NOT BAD

#define T_SAMPLING 500/2 //useconds, 2000Hz
#define BUFFER 2

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

bool state = true; 
unsigned int last_error = 0;
unsigned long last_us = 0L;
int i = 0;


void setup() {
  
  pinMode(3, OUTPUT);
}


void loop(void){  
     while(i< BUFFER){
    if (micros() - last_us > T_SAMPLING){
      last_us += T_SAMPLING;

      digitalWrite(3, state);
      state = !state;
      last_error = sampling_delay(last_error);
      i++;
    }
 }
 
 last_error =  sending_delay(last_error);
 i = 0;
}

