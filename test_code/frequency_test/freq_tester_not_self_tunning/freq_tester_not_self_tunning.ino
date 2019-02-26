
/*Code to test the frequency perceived by the python
 * code, it just samples from the arduino ADC.
 * Look  for the sending code if you want the receiver.
 * Author:  Ithallo J. A. Guimaraes
 This code implements a self tunning data sampler
 */

//#define DEBUG //uncomment fo debugging
#define TWOBYTES // uncomment to use the two bytes through USB sending protocol

#define BUFFER_SIZE 20
#define DELTA 500 //us, 2000Hz 
#define ERROR_TIME 144 //us manually adjusted
#define DELTA_TIME DELTA - ERROR_TIME // us

byte data[BUFFER_SIZE];
unsigned int time_delta [BUFFER_SIZE/2];
unsigned int value = 0;
unsigned int t1, t2;
unsigned int i = 0;
unsigned int j = 0;

void setup() {
 
  Serial.begin(115200);
  delay(1000);
  #ifdef DEBUG
    Serial.println("Starting RX");
  #endif 
  
}

void loop(void){
   i = 0;
   j = 0;
   
   while(i < (BUFFER_SIZE)){
        t1 = micros();
        delayMicroseconds(DELTA_TIME);// its lazy but it works fine
        value = analogRead(0);
        time_delta[j] = t2;
        
        data[i] = (value>>8);
        data[i+1] = (value&255);
        i += 2;
        j++;
        t2 = micros() - t1;
      
   }
      j=0;
      for (i=0;i<(BUFFER_SIZE);i+=2){
        
          value = (data[i]<<8) | (data[i+1]&255);
          #ifdef DEBUG
            Serial.print("Value: ");
            Serial.print(value);
            Serial.print(" || Time: ");
            Serial.print(time_delta[j]);
            Serial.print("us ");
            Serial.print(" || DELTA Time: ");
            Serial.print(DELTA_TIME);
            Serial.println("us ");
            j++;
          #else 
              #ifdef TWOBYTES
                Serial.write(value/256);
                Serial.write(value%256);
              #else
                Serial.println((value/1023.)*5.);
              #endif
          #endif
        }  
   
  }


