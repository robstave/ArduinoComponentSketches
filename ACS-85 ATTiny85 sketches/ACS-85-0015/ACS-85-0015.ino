
/**
 * ACS-85-0015
 * ATTiny85  Dual LFO Two Ranges
 *
 * Two LFO taken from 85-0004
 * Just implements LF1, LF2, one for each pin.
 * Simple, utility sketch.
 *  
 *
 * External pin 1       = Reset (not used)
 * External pin 2 (PB3) = input 0 freq 1
 * External pin 3 (PB4) = input 1 freq 2
 * External pin 4       = GND
 * External pin 5 (PB0) = output 0 output
 * External pin 6 (PB1) = Output 1 output
 * External pin 7 (PB2) = none
 * External pin 8       = Vcc
 *
 * V 1.0  -  First Version
 *
 * Note: This sketch has been written specifically for ATTINY85 and not Arduino uno
 * Observations.
 *
 * Its a copy/paste job..so there certainly can be improving to be had.
 * Rob Stave (Rob the fiddler) ccby 2015
 * 
 * TODO..ditch the nested loops and just scale the counter.
 */



//  ATTiny overview
//                           +-\/-+
//                    Reset 1|    |8  VCC
//      (pin3) in 0 A3  PB3 2|    |7  PB2 (pin2) none
//      (pin4) in 1 A2  PB4 3|    |6  PB1 (pin1) out 1
//                      GND 4|    |5  PB0 (pin0) out 0
//                           ------



/**
 * Note in the switch, we change it up with an extra
 * nested loop of 50
 *
 * so
 * 8mhz/2/100/50 = 800hz Interrupt freq
 */

//25 -> 800/2 = 16
// 400  -> 1
#define VCO1_L1_HIGH 25
#define VCO1_L1_LOW 400

//100 -> 4
//2000 -> .2

#define VCO2_L2_HIGH 100
#define VCO2_L2_LOW 2000



//counters for the frequencies
int oscFreq1 = 0;
int oscCounter1 = 0;
int oscFreq2 = 0;
int oscCounter2 = 0;


int lfoCounter = 0;

 
int loopCount = 0;


// the setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00000011;  //set output bits

  // initialize timer1
  noInterrupts();           // disable all interrupts

  TCCR1 = 0;                  //stop the timer
  TCNT1 = 0;                  //zero the timer
  //GTCCR = _BV(PSR1);          //reset the prescaler
  OCR1A = 99;                //set the compare value
  OCR1C = 99;
  TIMSK = _BV(OCIE1A);        //interrupt on Compare Match A

  TCCR1 = _BV(CTC1) | _BV(CS11); // Start timer, ctc mode, prescaler clk/2

  interrupts();             // enable all interrupts

}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{


  //Count up and toggle portB bits
  if (lfoCounter > 50) {
    lfoCounter = 0;

    //Count up and toggle portB bits
    if (oscCounter1 > oscFreq1) {
      oscCounter1 = 0;
      PORTB ^= (_BV(PB0));
    }
    oscCounter1++;

    if (oscCounter2 > oscFreq2) {
      oscCounter2 = 0;
      PORTB ^= (_BV(PB1));
    }
    oscCounter2++;

  }
  lfoCounter++;

}

void loop() {

  int osc1_t = analogRead(A3);
  int osc2_t = analogRead(A2);

  oscFreq1 = map(osc1_t, 0, 1023, VCO1_L1_LOW,  VCO1_L1_HIGH);
  oscFreq2 = map(osc2_t, 0, 1023, VCO2_L2_LOW,  VCO2_L2_HIGH);

}
