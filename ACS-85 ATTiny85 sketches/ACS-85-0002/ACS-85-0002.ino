
/**
 * ACS-85-0002
 * ATTiny85  simple vcos
 *
 * 2 inputs and 3 outputs.
 *
 * External pin 1       = Reset (not used)
 * External pin 2 (PB3) = input 0
 * External pin 3 (PB4) = input 1
 * External pin 4       = GND
 * External pin 5 (PB0) = output 0
 * External pin 6 (PB1) = output 1
 * External pin 7 (PB2) = out mixed
 * External pin 8       = Vcc
 *
 * Timer 1 is used in this case.
 * the prescaler is ck1  /2 and the ocr1 is 100
 * so a osc value of 50 should be about 400 hz.  This is
 * what I get on a scope.
 * 
 * see   // see http://www.atmel.com/Images/atmel-2586-avr-8-bit-microcontroller-attiny25-attiny45-attiny85_datasheet.pdf
 *
 * See table 13-5
 * CS13, CS12, CS11, CS10 = [ 0, 0, 1, 0] is prescaler/2
 * 
 * The other pin (7) is just a fun mix between the two.
 * 
 * Feel free to tweek the ranges and timers to get more
 * resolution.
 * in this case the range is 7 - 180
 * 
 *  7   = 2857
 *  8   = 2500
 *  30  = 666.7
 *  31  = 645.1
 *  40  = 500 hz
 *  41  = 487.8
 *  50  = 400.0 hz
 *  100 = 200 hz
 *  120 = 166.7
 *  121 = 165.3 hz
 *  150 = 133,3 hz
 *  151 = 132.4 hz
 *  180 = 111.1 hz
 *  
 *  notice that the space between freqs gets bigger as
 *  the freq gets higher.  Such a wide range can cause problems.
 *  Of course, speeding the clock increases the resolution, but the
 *  faster the interrupt, the less you can do as well.
 *  
 *  Consider using PWM and outputing the timer directly out.
 *  (that is another sketch)
 *  
 * 
 * V 1.0  -  First Version
 * V 1.1  -  Comments
 * 
 * TODO, add Hysteresis 
 *
 * Note: This sketch has been written specifically for ATTINY85 and not Arduino uno
 *
 * Rob Stave (Rob the fiddler) CCBY 2015
 */



//  ATTiny overview
//                           +-\/-+
//                    Reset 1|    |8  VCC
//      (pin3) in 0 A3  PB3 2|    |7  PB2 (pin2) out mixed
//      (pin4) in 1 A2  PB4 3|    |6  PB1 (pin1) out 1
//                      GND 4|    |5  PB0 (pin0) out 0
//                           ------

//Ranges for the pot.  Technically a small nuumber means a 
//shorter timer so low or high...whatever you want to call it.


#define VCO1HIGH 7
#define VCO1LOW 108

#define VCO2HIGH 7
#define VCO2LOW 108

// Make bigger for longer alt sounds on PB2
#define MIX_LFO 2000  

//counters for the frequencies
int oscFreq1 = 0;
int oscCounter1 = 0;
int oscFreq2 = 0;
int oscCounter2 = 0;

boolean oscState = false;
int oscCounter3 = 0;

// the setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00000111;  //could use this

  // initialize timer1
  noInterrupts();           // disable all interrupts

  TCCR1 = 0;                  //stop the timer
  TCNT1 = 0;                  //zero the timer

  OCR1A = 100;                //set the compare value
  OCR1C = 100;                //set the compare value

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

void loop() {

  int osc1_t = analogRead(A3);
  oscFreq1 = map(osc1_t, 0, 1023, VCO1LOW ,  VCO1HIGH);
  osc1_t = analogRead(A2);
  oscFreq2 = map(osc1_t, 0, 1023, VCO2LOW,  VCO2HIGH);

}
