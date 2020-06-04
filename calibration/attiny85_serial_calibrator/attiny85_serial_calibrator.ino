/*
A serial to the attiny85
that will be used to calibrate its internal oscillator.
It writes the callibration factor to the eeprom starting
at 0.
 */
#include <EEPROM.h>
#include<SoftwareSerial.h>

#define PIN 0
#define HALF_CYCLE 500 //useconds, 1kHz

const byte RX=4;
const byte TX=3;
unsigned long int t=0;
bool initial_state=0;
char response='g';
char value='0';

SoftwareSerial mySerial(RX, TX);


void setup() {
  pinMode(PIN, OUTPUT);
  pinMode(RX, INPUT);
  pinMode(TX, OUTPUT);

  mySerial.begin(9600);
  delay(1000);

  mySerial.println("Started");
  mySerial.println("disable line endings");
  mySerial.println("(s)tart");
  mySerial.println("(i)ncrease OSCCAL");
  mySerial.println("(d)ecrease OSCCAL");
  mySerial.println("(R)ead/(W)rite OSCCAL from/to EEPROM");

  mySerial.print("OSCCAL at beginning: ");
  mySerial.println(OSCCAL);
  
  delay(1000);
  
}


void loop(void){  

  while (mySerial.available() > 0) {
    
    response = mySerial.read();
    mySerial.print("echo: ");
    mySerial.println(response);

    if (response=='b'){
      mySerial.println("signal begin");
      break;
    }
    else if(response=='i'){
      
      mySerial.print("OSCCAL before: ");
      mySerial.println(OSCCAL);
      
      OSCCAL +=1;
      response = 'b';
    }

    else if(response=='d'){
      
      OSCCAL -=1;     
      response = 'b';
    }
    
    else if (response=='R'){
      
      EEPROM.get(0, value);
      
      mySerial.print("read from EEPROM: ");
      mySerial.println( (int)value );

      response = 'b';
    }

    else if (response=='W'){
      
      EEPROM.update(0, (byte) OSCCAL);

      mySerial.println("written to EEPROM");
      response = 'b';
    }
    else {
       mySerial.println("signal stop");
    }

    mySerial.print("OSCCAL now: ");
    mySerial.println(OSCCAL);  

  }


  if (response=='b'){

    initial_state ^= 1;
    digitalWrite(PIN, initial_state);
  
    while( (micros() - t) <= HALF_CYCLE);
    t += HALF_CYCLE;
    }
}
