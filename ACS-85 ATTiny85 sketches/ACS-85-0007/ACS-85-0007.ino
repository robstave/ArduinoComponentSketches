
/**
   ACS-85-0007
   ATTiny85  Square wave with variable frequency.

   The period changes in this case using counters.
   Each time the pin flips, the period is reduced or increased.
   The size of the increase is masked by 3,4 or 5 bits. Since the
   period increases, the frequency decreases.  Its much more of rampy sound
   for PB2


   so for
   PB0, the period is n + (x mod 8)
   PB1, the period is n + (x mod 16)
   PB2, the period is n + (x mod 32)



   External pin 1       = Reset (not used)
   External pin 2 (PB3) = input 0 freq 1
   External pin 3 (PB4) = none
   External pin 4       = GND
   External pin 5 (PB0) = output 0 smaller variance
   External pin 6 (PB1) = Output 1
   External pin 7 (PB2) = Output 2 larger variance
   External pin 8       = Vcc


   V 1.0  -  First Version

   Note: This sketch has been written specifically for ATTINY85 and not Arduino uno.
     It might work, but you prob need to change Pins/ports

   Rob Stave (Rob the fiddler) ccby 2015
*/



//  ATTiny overview
//                      +-\/-+
//               Reset 1|    |8  VCC
// (pin3) in 0 A3  PB3 2|    |7  PB2 (pin2) out 2
// (pin4)    none  PB4 3|    |6  PB1 (pin1) out 1
//                 GND 4|    |5  PB0 (pin0) out 0
//                      ------


/**
 * Ranges for the counters.
 * A low value means a faster countdown and thus a higher frequency
 * 
 * 40 => 1250hz
 * 45 => 1,111 hz
 * 215 - > 232 hz
 * 222 => 225 hz
 *  Note: the counter is a byte and the ramp is max 5 bits = 32...so the VCO1_LOW should max out at 223
 */
 
#define VCO1_HIGH 40
#define VCO1_LOW 223

//counters for the frequencies
volatile uint8_t oscFreq1 = VCO1_LOW;

volatile uint8_t oscCounter1 = VCO1_LOW;
volatile uint8_t oscCounter2 = VCO1_LOW;
volatile uint8_t oscCounter3 = VCO1_LOW;

volatile uint8_t counter1 = 0;
volatile uint8_t counter2 = 0;
volatile uint8_t counter3 = 0;

// the setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00000111;  //set output bits

  // initialize timer1
  noInterrupts();           // disable all interrupts

  TCCR0A = 0;
  TCCR0B = 0;

  TCCR0A |= (1 << WGM01); //Start timer 1 in CTC mode Table 11.5

  OCR0A = 79; //CTC Compare value

  TCCR0B |= (1 << CS00); // Prescaler =0 Table 11.6

  // TCCR0A |=(1<<COM0A1); //Timer0 in toggle mode Table 11.2
  TIMSK |= (1 << OCIE0A); //Enable CTC interrupt see 13.3.6
  interrupts();             // enable all interrupts
}

ISR(TIMER0_COMPA_vect)          // timer compare interrupt service routine
{

  //Count up and toggle portB bits
  if (oscCounter1 <= 0) {
    counter1++;
    oscCounter1 = oscFreq1 + ( counter1 & B00000111);
    PORTB ^= (_BV(PB0));
  }
  oscCounter1--;

  if (oscCounter2 <= 0) {
    counter2++;
    oscCounter2 = oscFreq1 + ( counter2 & B00001111);
    PORTB ^= (_BV(PB1));
  }
  oscCounter2--;

  if (oscCounter3 <= 0) {
    counter3++;
    oscCounter3 = oscFreq1 +  ( counter3 & B00011111);
    PORTB ^= (_BV(PB2));
  }
  oscCounter3--;

}

void loop() {

  //Read freq
  int osc1_t = analogRead(A3);
  oscFreq1 = map(osc1_t, 0, 1023, VCO1_LOW,  VCO1_HIGH);
}
