
/**
 * ACS-85-0006
 * ATTiny85  Squarewave with PWM period
 *
 *
 * External pin 1       = Reset (not used)
 * External pin 2 (PB3) = input 0 freq 1
 * External pin 3 (PB4) = none
 * External pin 4       = GND
 * External pin 5 (PB0) = output 0 output pure square
 * External pin 6 (PB1) = Output 1 output random period
 * External pin 7 (PB2) = Output 2 output additive period
 * External pin 8       = Vcc
 *

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
volatile byte oscFreq1 = 200;
volatile byte  oscCounter1 = 0;
volatile byte nextClock1 = 200;

volatile byte  oscCounter2 = 0;
volatile byte nextClock2 = 200;


volatile byte  oscCounter3 = 0;
volatile byte nextClock3 = 200;


volatile byte counter1= 1; 
volatile byte counter2= 1;
volatile byte counter3= 1;
 


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
  if (oscCounter1 > nextClock1) {
    oscCounter1 = 0;
     counter1++;
     nextClock1 = oscFreq1 + ( counter1 & B00000111);
     
    PORTB ^= (_BV(PB0));
  }
  oscCounter1++;

  if (oscCounter2 > nextClock2) {
    oscCounter2 = 0;
    
    counter2++;

    nextClock2 = oscFreq1 + ( counter2 & B00001111);
    PORTB ^= (_BV(PB1));
  }
  oscCounter2++;

  if (oscCounter3 > nextClock3) {
    oscCounter3 = 0;

    counter3++;
    nextClock3 = oscFreq1 +  ( counter3 & B00011111);
    PORTB ^= (_BV(PB2));
  }
  oscCounter3++;

}


void loop() {

  //Read freq
  int osc1_t = analogRead(A3);
  oscFreq1 = map(osc1_t, 0, 1023, VCO1_LOW,  VCO1_HIGH);
 

}
