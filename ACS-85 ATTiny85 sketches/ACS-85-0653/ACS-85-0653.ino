/**
   ACS-85-0653
   ATTiny85  Bytebeat

   Bytebeat  simple 1 bit version 2

   based on  https://www.noizhardware.com/

   https://www.reddit.com/r/diydsp/comments/ksgn2s/bytebeat_synth_on_a_single_attiny85/
   code used in jamuary02 - Bytebeat
     https://youtu.be/8HQUXlStqK8
     schematic at https://imgur.com/SekUTgN
     noizhardware.com
     enjoy :)

   External pin 1       = Reset (not used)
   External pin 2 (PB3) = input 0 Speed/freq
   External pin 3 (PB4) = input 1 Equation
   External pin 4       = GND
   External pin 5 (PB0) = output 2
   External pin 6 (PB1) = output 3
   External pin 7 (PB2) = output 4
   External pin 8       = Vcc

   V 1.0  -  First Version
   V 1.1  -  Fixed Comments
   Rob Stave (Rob the fiddler) ccby 2021
*/

//  ATTiny overview
//                           +-\/-+
//                    Reset 1|    |8  VCC
//      (pin3) freq A3  PB3 2|    |7  PB2 (pin2) output 3
//  (pin4) Equation A2  PB4 3|    |6  PB1 (pin1) output 2
//                      GND 4|    |5  PB0 (pin0) output 1
//

#include <avr/io.h>

#define OUT1 0  /* pin5 */
#define OUT2 1  /* pin6 */
#define OUT3 2  /* pin7 */
#define POT_B 4 /* pin3 */
#define POT_A 3 /* pin2 */

volatile uint16_t t = 0;
volatile uint8_t out = 0;
volatile uint8_t PARAM_A;
volatile uint8_t EQ = 0;
void setup()
{

  DDRB = B00000111; // set output bits

  // Enable 64 MHz PLL and use as source for Timer1
  // PLLCSR = 1 << PCKE | 1 << PLLE;

  noInterrupts(); // disable all interrupts

  TCCR0A = 0;
  TCCR0B = 0;

  TCCR0A |= (1 << WGM01); // Start timer 1 in CTC mode Table 11.5

  OCR0A = 5; // CTC Compare value...this is fairly arbitrary and you can change, but have to adjust math.

  // TCCR0B |= (1 << CS00) | (1 << CS01); // Prescaler =64 Table 11.6
  TCCR0B |= (1 << CS02);  // Prescaler =64 Table 11.6
  TIMSK |= (1 << OCIE0A); // Enable CTC interrupt see 13.3.6
  interrupts();           // enable all interrupts
}

ISR(TIMER0_COMPA_vect)
{

  t++;

  if (EQ < 3)
  {
    out = t & 4096 ? (t * (t ^ t % 255) | t >> 4) >> 1 : t >> 3 | (t & 8192 ? t << 2 : t);
    return;
  }

  if (EQ < 6)
  {
    out = t ^ t >> 4 ^ (t >> 11 + (t >> 16) % 3) % 16 * t ^ 3 * t;
    return;
  }

  if (EQ < 8)
  {

    out = t * (t >> 11 & t >> 8 & 123 & t >> 3);
    return;
  }

  if (EQ < 12)
  {
    out = t * (42 & t >> 10);
    return;
  }

  if (EQ < 15)
  {
    out = t * (t >> 5 | t >> 8) >> (t >> 16);
    return;
  }

  if (EQ < 18)
  {
    out = t * (t >> 8 * (t >> 15 | t >> 8) & (20 | 5 * (t >> 19) >> t | t >> 3));
    return;
  }

  out = t >> 9 & 2 * t & 11 * t | t >> 7 & 4 * t;
}

void loop()
{

  uint8_t readLoop = 0;
 
  while (true)
  {

    readLoop++;

    if (readLoop == 3)
    {
      // speed
      OCR0A = map(analogRead(A3), 0, 1023, 3, 254);
    }

    // select equation
    if (readLoop > 8)
    {
      EQ = map(analogRead(A2), 0, 1023, 0, 23);
      readLoop = 0;
    }

    // if ( out == 0) { //sucks
    if (bitRead(out, 1) > 0)
    {

      bitSet(PORTB, OUT1);
    }
    else
    {
      bitClear(PORTB, OUT1);
    }

    if (bitRead(out, 2) > 0)
    {
      bitSet(PORTB, OUT2);
    }
    else
    {
      bitClear(PORTB, OUT2);
    }

    if (bitRead(out, 3) > 0)
    {
      bitSet(PORTB, OUT3);
    }
    else
    {
      bitClear(PORTB, OUT3);
    }
  }
}
