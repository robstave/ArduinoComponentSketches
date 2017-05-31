
/**
 * ACS-85-0004
 * ATTiny85  Tri VCO With Detune
 *
 *
 * External pin 1       = Reset (not used)
 * External pin 2 (PB3) = input 0 freq 1
 * External pin 3 (PB4) = input 1 freq 2
 * External pin 4       = GND
 * External pin 5 (PB0) = output 0 output
 * External pin 6 (PB1) = Output 1 output
 * External pin 7 (PB2) = Output 2 output
 * External pin 8       = Vcc
 *
 *
 * Took several iterations to find the sweet spot.
 * The timer is not prescaled, so the interrupt is
 * really about as long as the value you set for OCR0A.
 * 
 * By this point, you can see that I get my frequencies with counters.
 * in this case, 8mhz/(79+1)/50/2 = 1khz
 * 
 * To get more resolution, you need a faster interrupt, but if its too fast, then
 * the size of the interrupt is too big and your top frequency actually goes down.
 * 
 * with int counters, I could not get above 750hz.  But I swapped the ints for bytes
 * and it saved several clocks
 * 
 * 
 * V 1.0  -  First Version
 * 
 * 
 *
 * Note: This sketch has been written specifically for ATTINY85 and not Arduino uno
 * Observations.
 *
 * Rob Stave (Rob the fiddler) ccby 2015
 */



//  ATTiny overview
//                           +-\/-+
//                    Reset 1|    |8  VCC
//      (pin3) in 0 A3  PB3 2|    |7  PB2 (pin2) out 2
//      (pin4) in 1 A2  PB4 3|    |6  PB1 (pin1) out 1
//                      GND 4|    |5  PB0 (pin0) out 0
//                           ------



// 30 -> 666 hz
// 120 - > 166 hz
#define VCO1_HIGH 50
#define VCO1_LOW 200
 



//counters for the frequencies
 byte oscFreq1 = 200;
 byte  oscCounter1 = 0;
 byte  oscFreq2 = 200;
 byte  oscCounter2 = 0;
 byte  oscFreq3 = 200;
 byte  oscCounter3 = 0;
 
// the setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00000111;  //set output bits

  // initialize timer1
  noInterrupts();           // disable all interrupts

  TCCR0A = 0;
  TCCR0B = 0;

  TCCR0A |= (1 << WGM01); //Start timer 1 in CTC mode Table 11.5

  OCR0A = 79; //CTC Compare value

  TCCR0B |= (1 << CS00); // Prescaler =8 Table 11.6

  // TCCR0A |=(1<<COM0A1); //Timer0 in toggle mode Table 11.2
  TIMSK |= (1 << OCIE0A); //Enable CTC interrupt see 13.3.6
  interrupts();             // enable all interrupts
}

ISR(TIMER0_COMPA_vect)          // timer compare interrupt service routine
{

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

  if (oscCounter3 > oscFreq3) {
    oscCounter3 = 0;
    PORTB ^= (_BV(PB2));
  }
  oscCounter3++;
  
}

int detuneCount = 0;

void loop() {

  //Read freq 
  int osc1_t = analogRead(A3);
  oscFreq1 = map(osc1_t, 0, 1023, VCO1_LOW,  VCO1_HIGH);

  //Read detune.  No need to check as often. I would rather
  //the freq be checked more often.
  if (detuneCount > 5) {
    detuneCount = 0;
    int osc2_t = analogRead(A2);
    int detune  = map(osc2_t, 0, 1023, 0,  20);
    oscFreq2 = oscFreq1 + detune;
     oscFreq3 = oscFreq2 + detune;
  }
  detuneCount++;

}
