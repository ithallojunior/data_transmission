/* Serial trhoughput tester
 *  It tests the diferent ways of sending serial data
 */


#define VALUE 1021 // a random value
#define TOTAL_EXEC 1000 
#define BAUD 115200 


unsigned int Max_straight = 0; // just sent
unsigned int Min_straight = 100000; // just sent

unsigned int Max_with_division = 0; // using two divisions
unsigned int Min_with_division = 100000; // using two divisions

unsigned int Max_with_shift = 0; // using bitwise shift
unsigned int Min_with_shift = 100000; // bitwise shift


unsigned long start_;
unsigned long delta;

float total_straight = 0;
float total_with_division = 0;
float total_with_shift = 0;

uint16_t i=0;

void setup() {
  // serial settings
  Serial.begin(BAUD);
  delay(100);
  Serial.println("Starting");

  //the loop mus be here as it needs to do something and stop doing it

  //straight
  for(i=0; i < TOTAL_EXEC; i++){
   
   start_  = micros(); 
   Serial.write(VALUE);
   Serial.write("\n");
   delta = micros() - start_;
   
   total_straight += delta;

   //ifs for max min
   if (delta > Max_straight)
      Max_straight = delta;
   if (delta < Min_straight)
     Min_straight = delta;
   }



  //division
  for(i=0; i < TOTAL_EXEC; i++){
   
   start_  = micros(); 
   Serial.write(VALUE/256);
   Serial.write(VALUE%256);
   delta = micros() - start_;
   total_with_division += delta;

   //ifs for max min
   if (delta > Max_with_division)
      Max_with_division = delta;
   if (delta < Min_with_division)
     Min_with_division = delta;
   }


  //shift
  for(i=0; i < TOTAL_EXEC; i++){
   
   start_  = micros(); 
   Serial.write(VALUE>>8);
   Serial.write(VALUE&255);
   delta = micros() - start_;
   
   total_with_shift += delta;

   //ifs for max min
   if (delta > Max_with_shift)
      Max_with_shift = delta;
   if (delta < Min_with_shift)
     Min_with_shift = delta;
   }




  // the results
  Serial.println("\n\nFinished");
  Serial.print("Baudrate: ");
  Serial.print(BAUD);
  Serial.print("| Total runs: ");
  Serial.println(TOTAL_EXEC);

  
  Serial.print("Time sending just with line breaker (us): ");
  Serial.print("Total time: ");
  Serial.print(total_straight);
  Serial.print("| Mean: ");
  Serial.print(total_straight/(float)TOTAL_EXEC);
  Serial.print("| Max.: ");
  Serial.print(Max_straight);
  Serial.print("| Min.: ");
  Serial.println(Min_straight);


  Serial.print("Time using division (us): ");
  Serial.print("Total time: ");
  Serial.print(total_with_division);
  Serial.print("| Mean: ");
  Serial.print(total_with_division/(float)TOTAL_EXEC);
  Serial.print("| Max.: ");
  Serial.print(Max_with_division);
  Serial.print("| Min.: ");
  Serial.println(Min_with_division);


  Serial.print("Time using shift (us): ");
  Serial.print("Total time: ");
  Serial.print(total_with_shift);
  Serial.print("| Mean: ");
  Serial.print(total_with_shift/(float)TOTAL_EXEC);
  Serial.print("| Max.: ");
  Serial.print(Max_with_shift);
  Serial.print("| Min.: ");
  Serial.println(Min_with_shift);
}


void loop(void){
  //yes, it is empty
  }
