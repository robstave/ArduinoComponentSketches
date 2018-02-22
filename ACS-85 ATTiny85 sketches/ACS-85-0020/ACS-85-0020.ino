
/**
   ACS-85-0020
   ATTiny85  LFO Squarewave with adjustable duty cycle

   External pin 1       = Reset (not used)
   External pin 2 (PB3) = input 0 freq 1
   External pin 3 (PB4) = input 1 duty
   External pin 4       = GND
   External pin 5 (PB0) = output 0 output
   External pin 6 (PB1) = output 1 !output
   External pin 7 (PB2) = output 2 1/2 freq
   External pin 8       = Vcc

   V 1.0  -  First Version

   Rob Stave (Rob the fiddler) ccby 2015
*/

//  ATTiny overview
//                      +-\/-+
//               Reset 1|    |8  VCC
// (pin3) in 0 A3  PB3 2|    |7  PB2 (pin2) .5 freq
// (pin4) in 1 A2  PB4 3|    |6  PB1 (pin1) out 1
//                 GND 4|    |5  PB0 (pin0) out 0
//                      ------

//Ranges for the pot.  Technically a small number means a
//shorter timer so low or high...whatever you want to call it.

#define VCO1_L1_HIGH 500
#define VCO1_L1_LOW 8000

//counters for the frequencies
volatile uint16_t dutyOn = 0;
volatile uint16_t dutyOff = 0;
volatile uint16_t oscCounter1 = 0;

volatile boolean doHigh = HIGH;

// the setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00000111;  //set output bits

  // initialize timer1
  noInterrupts();           // disable all interrupts

  TCCR1 = 0;                  //stop the timer
  TCNT1 = 0;                  //zero the timer
  //GTCCR = _BV(PSR1);          //reset the prescaler
  OCR1A = 99;                //set the compare value
  OCR1C = 99;
  TIMSK = _BV(OCIE1A);        //interrupt on Compare Match A

  TCCR1 = _BV(CTC1)  | _BV(CS11) | _BV(CS12); // Start timer, ctc mode, prescaler clk/32

  interrupts();             // enable all interrupts
}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{

  if (doHigh == HIGH) {
    //Count up and toggle portB bits
    if (oscCounter1 >= dutyOn) {
      oscCounter1 = 0;
      PORTB &= ~_BV(PB0);
      PORTB |= _BV(PB1);
      PORTB ^= (_BV(PB2));
      doHigh = LOW;
    }

  } else {
    if (oscCounter1 >= dutyOff) {
      oscCounter1 = 0;
      PORTB |= _BV(PB0);
      PORTB &= ~_BV(PB1);
      // PORTB ^= (_BV(PB2));
      doHigh = HIGH;
    }
  }

  oscCounter1++;
  return;

}

void loop() {

  int osc1_t = analogRead(A3);
  int duty_t = analogRead(A2);


  long count = map(osc1_t, 0, 1023, VCO1_L1_LOW,  VCO1_L1_HIGH);
  long duty  = map(duty_t, 0, 1023, 5,  95);

  long calcDutyOn = (count * duty) / 100;

  dutyOn = calcDutyOn;
  dutyOff  = (count - dutyOn);

  return;
}
