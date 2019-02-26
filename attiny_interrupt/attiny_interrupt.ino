/*Simple Interrupt code, to show how to use it
 *  Freq: 2000Hz
 * For more:
 * https://embeddedthoughts.com/2016/06/06/attiny85-introduction-to-pin
 * -change-and-timer-interrupts/
 * https://www.avrfreaks.net/forum/timer-setup-attiny85
 */



//#include <avr/sleep.h>
//#include <avr/power.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//unsigned int i = 0;

ISR(TIMER0_COMPA_vect) {
  
  //if(i == 4){
   PORTB ^= (1 << PB3); //toggles PB3 on and off
  //i = 0;
  //}
  //else{
  //  i++;
  //}

}


int main(void) {
  
  
  DDRB = 0b001000; // PB3  as output
  PORTB = 0b000000; //|= (1 << PB4); //disable pullup
  
  TCCR0A = (1 << WGM01);             //CTC mode
  TCCR0B = (2 << CS00);              //div8
  OCR0A =  F_CPU/8 * 0.00050 - 1;//2kHz //F_CPU/8 * 0.000050 - 1;  // 10kHz, precisely 10,22kHz
  TIMSK |= (1<<OCIE0A);              //if you want interrupt, 

  //enable compare match interrupt

  sei();
  for(;;){
  }
  
  return 0;
}


