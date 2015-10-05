
#include            <avr/io.h>                                                                                                                                                                                                                                                                                                                                                                         /*            attiny            register            names            */
#include            <avr/interrupt.h>

/**
 * ATTiny84  Simple Oscillator chip - Beeps.
 * Not exactly meant for production, mostly a workbench helper
 * to provide 40106-like signals without all the capacitors
 * and such.  Just drop it into a 5v circuit and away you no.
 * Nothing is configurable...however feel free to tweek code as needed.
 *
 * External pin 2 =  LFO (Trigger by arp array size)
 * External pin 3 =  LFO (Trigger by swell array size)
 * External pin 5 =  LFO (med freq, mix of Random 3)
 * External pin 6 = Rand notes
 * External pin 7 = Rand notes2
 * External pin 8 = Arp1
 * External pin 9 = Arp2
 * External pin 10 = Swell
 * External pin 11 = Swell high
 * External pin 12 = Swell low
 * External pin 13 = Random notes 3
 *
 * Explanation.
 * This is built around timer0 interrupts.  There are many ways to do this.
 * I find that counting up and comparing (CTC) gives me a little more flexibility.
 *
 * The prescaler is set to 64  (CS00 and CS001)
 * see http://www.atmel.com/Images/atmel-2586-avr-8-bit-microcontroller-attiny25-attiny45-attiny85_datasheet.pdf
 * or google ATTINY85 data sheet and look for that table.
 *
 * assuming 8Mhz/(64*(OCR0A +1)) = 8mhz/(64*10) = 12.5k
 * So we are interrupting at 12.5Khz
 *
 *
 * V 1.0  -  First Version
 *
 * Rob Stave - Rob the fiddler - ccby 2015
 *
 */


//  ATTiny overview
//                           +-\/-+
//                   Vcc    1|    |14  GND
//                LFO  PB0  2|    |13  PA0  Rand
//                LFO  PB1  3|    |12  PA1  swell -
//               Reset PB3  4|    |11  PA2  swell +
//                LFO  PB2  5|    |10  PA3  swell
//                Rand PA7  6|    |9   PA4  Arp 2
//                Rand PA6  7|    |8   PA5  Arp 1

//Counters in the interrupt to toggle pins.
// Increasing the number reduces the frequency.


#define MAX_ARRAY_SIZE  64
int freqArray[] = {16,  17,  18,  38,  14,
                   39,  18,  37,  26,  36,
                   25,  27,  24,  18,  33,
                   20,  24,  20,  31,  35,
                   22,  34,  27,  31,  37,
                   23,  30,  19,  24,  37,
                   14,  25,  38,  36,  18,
                   20,  37,  21,  23,  29,
                   27,  16,  30,  14,  33,
                   16,  23,  23,  39,  37,
                   35,  38,  31,  21,  29,
                   24,  14,  35,  18,  31,
                   25,  27,  32,  32
                  };

int freq1ArrayIndex = 0;
int HF1_ALT_LFO =   900;
int hf1AltLfoCounter = 0;
int hf1AltCounter = 0;


int freq2ArrayIndex = 0;
int HF2_ALT_LFO =   1500;
int hf2AltLfoCounter = 0;
int hf2AltCounter = 0;






#define MAX_ARRAY_SIZE2  24
int freqArray2[] = {16,  18,  16,  20,  16, 22,
                    16,  24,  16,  26,  16, 28,
                    16,  30,  16,  32,  16, 34,
                    16,  36,  16,  38,  16, 40
                   };

int freq3ArrayIndex = 0;
int HF3_ALT_LFO =   800;
int hf3AltLfoCounter = 0;
int hf3AltCounter = 0;
int hf3AltCounter2 = 0;



#define MAX_ARRAY_SIZE3  42
int freqArray3[] = {30,  31,  29,  32, 28, 33,  
                    27,  34,  26,  35, 25, 36,  
                    24,  37,  23,  38, 22, 39,  
                    21,  40,  20,  41, 19, 
                    41,  20,  40,  21, 39,  22,  
                    38,  23,  37,  24, 36,  25,  
                    35,  26,  34,  27, 33,  28,  
                    32,  29,  31

                   };

int freq4ArrayIndex = 0;
int HF4_ALT_LFO =   800;
int hf4AltLfoCounter = 0;
int hf4AltCounter = 0;
int hf4AltCounter2 = 0;
int hf4AltCounter3 = 0;




int HF5_ALT_LFO =   1900;
int hf5AltLfoCounter = 0;
int hf5AltLfoCounterTweek = 0;
int hf5AltCounter = 0;
int hf5Freq = 0;


int tempCounter = 0;
long x = 0;



// the setup function runs once when you press reset or power the board
void setup() {

  DDRA = B11111111; // set PORTA
  DDRB = B00000111; // set PORTB (digital 13~8) to outputs

  //DDRB = 0x3F;  could use this too

  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  //OSCCAL = 0x9F;

  // initialize timer0
  noInterrupts();           // disable all interrupts
  TCCR0A = 0;
  TCCR0B = 0;



  // Enable CTC Interrupt
  TIMSK0 |= (1 << OCIE0A);
  //disable overflow
  TIMSK0 &= ~(1 << TOIE0);

  OCR0A = 9;
  // Insert clock, with /64 prescale
  TCCR0B |= (1 << CS01);
  TCCR0B |= (1 << CS00);
  DDRA            =            0b11111111;

  TCCR0B   |=  (0 <<   WGM02);
  TCCR0A   |=  (1 << WGM01)  | (0 << WGM00);
  interrupts();             // enable all interrupts

}


ISR(TIM0_COMPA_vect)          // timer compare interrupt service routine
{
 
  
  /************************************************************
   * Randomish notes.  Really random from a table
   */
  //play note
  if (hf1AltCounter >= freqArray[freq1ArrayIndex]) {
    hf1AltCounter = 0;
    PORTA ^= (_BV(PA6));
  }
  hf1AltCounter++;

  //count note length
  if (hf1AltLfoCounter >= HF1_ALT_LFO) {
    hf1AltLfoCounter = 0;
    freq1ArrayIndex++;
    //new freq
    if (freq1ArrayIndex >= MAX_ARRAY_SIZE) {
      freq1ArrayIndex = 0;
    }

  }
  hf1AltLfoCounter++;


  //play note
  if (hf2AltCounter >= freqArray[freq2ArrayIndex]) {
    hf2AltCounter = 0;
    PORTA ^= (_BV(PA7));
  }
  hf2AltCounter++;

  //count note length
  if (hf2AltLfoCounter >= HF2_ALT_LFO) {
    hf2AltLfoCounter = 0;
    freq2ArrayIndex++;
    //new freq
    if (freq2ArrayIndex >= MAX_ARRAY_SIZE) {
      freq2ArrayIndex = 0;
      
    }

    

  }
  hf2AltLfoCounter++;


  /**********************************************************
   * appegiated notes
   */
  //play note
  if (hf3AltCounter >= freqArray2[freq3ArrayIndex]) {
    hf3AltCounter = 0;
    PORTA ^= (_BV(PA5));
  }
  hf3AltCounter++;

  //play note
  if (hf3AltCounter2 >= freqArray2[MAX_ARRAY_SIZE2 - freq3ArrayIndex - 1]) {
    hf3AltCounter2 = 0;
    PORTA ^= (_BV(PA4));
    
  }
  hf3AltCounter2++;


  //count note length
  if (hf3AltLfoCounter >= HF3_ALT_LFO) {
    hf3AltLfoCounter = 0;
    freq3ArrayIndex++;
    //new freq
    if (freq3ArrayIndex >= (MAX_ARRAY_SIZE2)) {
      freq3ArrayIndex = 0;
      PORTB ^= (_BV(PB0));                 //Tucked a bit flip here for LFO 0
    }
    
  }
  hf3AltLfoCounter++;

/*************************************************
 * 
 * Swell
 */

  //play note
  if (hf4AltCounter >= freqArray3[freq4ArrayIndex]) {
    hf4AltCounter = 0;
    PORTA ^= (_BV(PA3));
  }
  hf4AltCounter++;

    //play note
  if (hf4AltCounter2 >= (freqArray3[freq4ArrayIndex]-8)) {
    hf4AltCounter2 = 0;
    PORTA ^= (_BV(PA2));
  }
  hf4AltCounter2++;

      //play note
  if (hf4AltCounter3 >= (freqArray3[freq4ArrayIndex]+8)) {
    hf4AltCounter3 = 0;
    PORTA ^= (_BV(PA1));
  }
  hf4AltCounter3++;

  
  //count note length
  if (hf4AltLfoCounter >= HF4_ALT_LFO) {
    hf4AltLfoCounter = 0;
    freq4ArrayIndex++;
    //new freq
    if (freq4ArrayIndex >= (MAX_ARRAY_SIZE3)) {
      freq4ArrayIndex = 0;
    }

    PORTB ^= (_BV(PB1));

  }
  hf4AltLfoCounter++;


/****
 * 
 * Kinda random notes witha  little variance in the note length
 * refers to table 1 and table 1 index
 * 
 */

 if (hf5AltCounter >= hf5Freq) {
    hf5AltCounter = 0;
    PORTA ^= (_BV(PA0));
  }
  hf5AltCounter++;

  
  //count note length
  if (hf5AltLfoCounter >= HF5_ALT_LFO) {
    hf5AltLfoCounter= 0;
    if (freq1ArrayIndex > MAX_ARRAY_SIZE/2) {
      hf5AltLfoCounter = HF5_ALT_LFO/2;
    }
    hf5Freq=freqArray[freq1ArrayIndex];

     PORTB ^= (_BV(PB2));
  }
  hf5AltLfoCounter++;


}




void loop() {
  //Do nothing
  // int a = 1 + 1;
  x++;
}
