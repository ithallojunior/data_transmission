
//#define DEBUG //uncomment fo debugging
#ifdef DEBUG
#else
  #define TWOBYTES // uncomment to use the two bytes through USB sending protocol
#endif

#define BUFFER_SIZE 16 //abs max per transmission 32Bytes, 16 for this
#define ANALOG_PIN 0 
#define DELTA 500 - 7 //usenconds, 2000Hz, error time


unsigned int value = 0;
unsigned long int t1, t2;

unsigned long int delta_t = DELTA;
unsigned long int delta_t2 = DELTA;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  #ifdef DEBUG
    Serial.println("Starting RX");
  #endif

}

void loop() {
  t1 = micros();
   value = analogRead(ANALOG_PIN);
    #ifdef TWOBYTES
      Serial.write(value/256);
      Serial.write(value%256);
    #endif
   t2 = micros() - t1;
   #ifdef DEBUG
      Serial.print("Value: ");
      Serial.print(value);
      Serial.print(" || Waiting Time: ");
      Serial.print(DELTA - t2);
      Serial.print("us ");
      Serial.print(" || t2: ");
      Serial.print(t2);
      Serial.println("us");
   #endif
   delayMicroseconds(DELTA - t2);
  
}
