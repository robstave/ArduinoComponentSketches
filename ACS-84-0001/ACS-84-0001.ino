
#include            <avr/io.h>                                                                                                                                                                                                                                                                                                                                                                         /*            attiny            register            names            */
#include            <avr/interrupt.h>

/**
 * ATTiny84  Simple Oscillator chip.
 *
 * Not exactly meant for production, mostly a workbench helper
 * to provide 40106-like signals without all the capacitors
 * and such.  Just drop it into a 5v circuit and away you no.
 * Nothing is configurable in real time...however feel free to tweek code as needed.
 *
 * External pin 2 = high Freq 0
 * External pin 3 = high freq 1
 * External pin 5 = high freq 2
 * External pin 6 = high freq 3
 * External pin 7 = sequence of frequencies
 * External pin 8 = Kinda a ramp/sweep
 * External pin 9 = Another sweep
 * External pin 10 = LFO 3
 * External pin 11 = LFO 2
 * External pin 12 = LFO 1
 * External pin 13 = LFO 0
 *
 * Explanation.
 * This is built around timer0 interrupts.  There are many ways to do this.
 * I find that counting up and comparing (CTC) gives me a little more flexibility.
 *
 * The prescaler is set to 64  (CS00 and CS001)
 * see http://www.atmel.com/Images/atmel-2586-avr-8-bit-microcontroller-attiny25-attiny45-attiny85_datasheet.pdf
 * or google ATTINY85 data sheet and look for that table.
 *
 * assuming 8Mhz/(64*(OCR0A +1)) = 8mhz/(64*10) = 12500
 * So we are interrupting at 12.5kKhz
 *
 * I have a note counter set at 32
 * toggling every 32 counts is 390 flips of the pin
 * which gives us a frequency of 191.  
 * My volt meter reads 195...so...close enough.  There is a tolerance of 10% without a
 * xtal.
 *
 * If you really want an accurate note, you need a crystal, bump it to 16mhz
 * and rather than use an interrupt/count strategy like this, output the overflow
 * directly to the pin.  Of course, this gives you only two tones.  Driving all
 * those pins in this manner is a bit much to ask and the trade off is accuracy.
 *
 * V 1.0  -  First Version
 *
 * Observations.
 * Had to use TIMER0_COMPA_vect vs TIM0_COMPA_vect
 * This is pushing it for the interrupt.
 * If you want higher resolution, you can drop OCRoA a bit and use
 * bigger count numbers, however, its hard to tell if your interrupt is
 * flowing into your next interrupt. Yo Dawg.  There is a few ways to 
 * tell without serial logs.  Things get distorted or you get a vibrato.
 * Also..you can comment sections of code and see if your frequencies change.
 * This is very observable if you change the prescaler to 8.  Removing
 * chunks of interrupt code will actually affect your freq, because the interrupt
 * overrflows into the next timer slice.
 *
 * Rob Stave (Rob the fiddler) 2015
 */


//  ATTiny overview
//                           +-\/-+
//                   Vcc    1|    |14  GND
//               HF0   PB0  2|    |13  PA0  LFO0
//               HF1   PB1  3|    |12  PA1  LFO1
//               Reset PB3  4|    |11  PA2  LFO2
//               HF2   PB2  5|    |10  PA3  LFO3
//               HF3   PA7  6|    |9   PA4  HF RAMP1
//             HFALT   PA6  7|    |8   PA5  HF RAMP2

//Counters in the interrupt to toggle pins.
// Increasing the number reduces the frequency.
#define HIGH_FREQ_0 32
#define HIGH_FREQ_1 28
#define HIGH_FREQ_2 24
#define HIGH_FREQ_3 21

#define LOW_FREQ_0 13416
#define LOW_FREQ_1 10440
#define LOW_FREQ_2  9416
#define LOW_FREQ_3  7440



//counters for the frequencies
int hf0Counter = 0;
int hf1Counter = 0;
int hf2Counter = 0;
int hf3Counter = 0;

int lfo0Counter = 0;
int lfo1Counter = 0;
int lfo2Counter = 0;
int lfo3Counter = 0;

//We play with this counters a little more
//for the sweep pin

#define HIGH_FREQ_1_INCREMENT 200
#define HF_RANGE_1_LO 40
#define HF_RANGE_1_HI 8
int hfRamp1Counter = 0;
int hfRamp1FreqCounter = 0;
int hfRamp1MaxCounter = HF_RANGE_1_LO;


#define HIGH_FREQ_2_INCREMENT 250
#define HF_RANGE_2_LO 30
#define HF_RANGE_2_HI 7
int hfRamp2Counter = 0;
int hfRamp2FreqCounter = 0;
int hfRamp2MaxCounter = HF_RANGE_2_LO;


/**
 * Pin A6 is just some tones.  They are defined in a array.
 * HF_ALT_LFO is the length of the tone.
 * the array is each frequency
 */
#define MAX_ARRAY_SIZE  21
int freqArray[] = {35, 17, 24, 34, 22, 27, 33, 21, 29, 16, 15, 32, 28, 19, 25, 26, 30, 20, 31, 22, 29, 14,};
int freqArrayIndex = 0;
int HF_ALT_LFO =   1200;
int hfAltLfoCounter = 0;
int hfAltCounter = 0;

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

  

  // initialize timer0
  noInterrupts();           // disable all interrupts
  TCCR0A = 0;
  TCCR0B = 0;

  // Enable CTC Interrupt
  TIMSK0 |= (1 << OCIE0A);

  TIMSK0 &= ~(1<<TOIE0);
  
  
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

  //Each interrupt..check the counters.  If there is a match,
  //reset and toggle the bit.
  if (lfo0Counter >= LOW_FREQ_0) {
    lfo0Counter = 0;
    PORTA ^= (_BV(PA0));
  }
  lfo0Counter++;

  if (lfo1Counter >= LOW_FREQ_1) {
    lfo1Counter = 0;
    PORTA ^= (_BV(PA1));
  }
  lfo1Counter++;

  if (lfo2Counter >= LOW_FREQ_2) {
    lfo2Counter = 0;
    PORTA ^= (_BV(PA2));
  }
  lfo2Counter++;

  if (lfo3Counter >= LOW_FREQ_3) {
    lfo3Counter = 0;
    PORTA ^= (_BV(PA3));
  }
  lfo3Counter++;


  //HF code is the same.  Just smaller counters
  if (hf0Counter >= HIGH_FREQ_0) {
    hf0Counter = 0;
    PORTB ^= (_BV(PB0));

  }
  hf0Counter++;

  if (hf1Counter >= HIGH_FREQ_1) {
    hf1Counter = 0;
    PORTB ^= (_BV(PB1));
  }
  hf1Counter++;

  if (hf2Counter >= HIGH_FREQ_2) {
    hf2Counter = 0;
    PORTB ^= (_BV(PB2));
  }
  hf2Counter++;

  if (hf3Counter >= HIGH_FREQ_3) {
    hf3Counter = 0;
    PORTA ^= (_BV(PA7));
  }
  hf3Counter++;


  //
  if (hfAltCounter >= freqArray[freqArrayIndex]) {
    hfAltCounter = 0;
    PORTA ^= (_BV(PA6));
  }
  hfAltCounter++;


  if (hfAltLfoCounter >= HF_ALT_LFO) {
    hfAltLfoCounter = 0;
    freqArrayIndex++;
    if (freqArrayIndex >= MAX_ARRAY_SIZE) {
      freqArrayIndex = 0;
    }

  }
  hfAltLfoCounter++;





  

  //Sweep is several nested loops
  if (hfRamp1Counter >= hfRamp1MaxCounter) {
    hfRamp1Counter = 0;
    PORTA ^= (_BV(PA4));
  }
  hfRamp1Counter++;

  //Adjust frequency
  if (hfRamp1FreqCounter >= HIGH_FREQ_1_INCREMENT) {
    hfRamp1FreqCounter = 0;
    hfRamp1MaxCounter--;
    if (hfRamp1MaxCounter < HF_RANGE_1_HI) {
      hfRamp1MaxCounter = HF_RANGE_1_LO;
    }
  }
  hfRamp1FreqCounter++;



  //Sweep is several nested loops
  if (hfRamp2Counter >= hfRamp2MaxCounter) {
    hfRamp2Counter = 0;
    PORTA ^= (_BV(PA5));
  }
  hfRamp2Counter++;

  //Adjust frequency
  if (hfRamp2FreqCounter >=HIGH_FREQ_2_INCREMENT) {
    hfRamp2FreqCounter = 0;
    hfRamp2MaxCounter--;
    if (hfRamp2MaxCounter < HF_RANGE_2_HI) {
      hfRamp2MaxCounter = HF_RANGE_2_LO;
    }
  }
  hfRamp2FreqCounter++;


}




void loop() {
  //Do nothing

}

// see http://ernstc.dk/arduino/tinytuner.html if your interested in ways to tune this better.
