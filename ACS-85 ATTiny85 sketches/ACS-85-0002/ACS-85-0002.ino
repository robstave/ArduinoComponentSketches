
/**
   ACS-85-0002
   ATTiny85 simple vcos

   2 inputs and 3 outputs.

   External pin 1       = Reset (not used)
   External pin 2 (PB3) = input 0
   External pin 3 (PB4) = input 1
   External pin 4       = GND
   External pin 5 (PB0) = output 0
   External pin 6 (PB1) = output 1
   External pin 7 (PB2) = out mixed
   External pin 8       = Vcc

   Timer 1 is used in this case.
   the prescaler is ck1  /2 and the ocr1 is 60

   see   // see http://www.atmel.com/Images/atmel-2586-avr-8-bit-microcontroller-attiny25-attiny45-attiny85_datasheet.pdf

   See table 13-5
   CS13, CS12, CS11, CS10 = [ 0, 0, 1, 0] is prescaler/2

   The other pin (7) is just a fun mix between the two.

   Feel free to tweek the ranges and timers to get more
   resolution.
   in this case the range is 7 - 180

   notice that the space between freqs gets bigger as
   the freq gets higher.  Such a wide range can cause problems.
   Of course, speeding the clock increases the resolution, but the
   faster the interrupt, the less you can do as well.

   Consider using PWM and outputing the timer directly out.
   (that is another sketch)

   V 1.0  -  First Version
   V 1.1  -  Comments
   v 1.2  -  Added average and tweeked number
   v 1.3  -  Small refactor

   Note: This sketch has been written specifically for ATTINY85 and not Arduino uno

   Rob Stave (Rob the fiddler) CCBY 2015
*/

//  ATTiny overview
//                      +-\/-+
//               Reset 1|    |8  VCC
// (pin3) in 0 A3  PB3 2|    |7  PB2 (pin2) out mixed
// (pin4) in 1 A2  PB4 3|    |6  PB1 (pin1) out 1
//                 GND 4|    |5  PB0 (pin0) out 0
//                      ------

//Ranges for the pot.  Technically a small number means a
//shorter timer so low or high...whatever you want to call it.

#define VCO1HIGH 5
#define VCO1LOW 108

#define VCO2HIGH 5
#define VCO2LOW 108

#define MIX_LFO 3000 // Make larger for longer alt sounds on PB2

//counters for the frequencies
byte oscFreq1 = 0;
byte oscCounter1 = 0;
byte oscFreq2 = 0;
byte oscCounter2 = 0;

boolean oscState = false;
int oscCounter3 = 0;

// Used for sample averaging
int f1Sample[4] = {0, 0, 0, 0};
int f2Sample[4] = {0, 0, 0, 0};
byte counter = 0;

// the setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00000111;  //could use this

  // initialize timer1
  noInterrupts();           // disable all interrupts

  TCCR1 = 0;                  //stop the timer
  TCNT1 = 0;                  //zero the timer

  OCR1A = 50;                //set the compare value
  OCR1C = 50;                //set the compare value  ??? needed

  TIMSK = _BV(OCIE1A);        //interrupt on Compare Match A

  TCCR1 = _BV(CTC1) | _BV(CS11); // Start timer, ctc mode, prescaler clk/2

  interrupts();             // enable all interrupts
}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{

  //Count up and toggle portB bits
  if (oscCounter1 > oscFreq1) {
    oscCounter1 = 0;
    PORTB ^= (_BV(PB0));

    //Bonus: toggle pin 2 also if state is true
    if (oscState == true) {
      PORTB ^= (_BV(PB2));
    }
  }
  oscCounter1++;

  if (oscCounter2 > oscFreq2) {
    oscCounter2 = 0;
    PORTB ^= (_BV(PB1));

    //Bonus: toggle pin 2 also if state is true
    if (oscState == true) {
      PORTB ^= (_BV(PB2));
    }
  }
  oscCounter2++;

  //toggle alternating frequency
  // So basically, we are setting a flag for on off.
  // If the state is on, the counters will sum together giving a ring effect.
  if (oscCounter3 > MIX_LFO) {
    oscCounter3 = 0;
    oscState = !oscState;
  }
  oscCounter3++;

}

void readVco1freq () {
  int osc1_t = 0;
  f1Sample[counter] = analogRead(A3);
  osc1_t = (f1Sample[0] + f1Sample[1] + f1Sample[2] + f1Sample[3]) >> 2;
  oscFreq1 = map(osc1_t, 0, 1023, VCO1LOW ,  VCO1HIGH);
}

void readVco2freq () {
  int osc1_t = 0;
  f2Sample[counter] = analogRead(A2);
  osc1_t = (f2Sample[0] + f2Sample[1] + f2Sample[2] + f2Sample[3]) >> 2;
  oscFreq2 = map(osc1_t, 0, 1023, VCO2LOW,  VCO2HIGH);
}

void tickCounter() {
  counter++;
  if (counter > 3) {
    counter = 0;
  }
}

void loop() {
  readVco1freq();
  readVco2freq();
  tickCounter();
}
