/**
   ACS-85-0016
   ATTiny85 Clock/Counter Baby 4
   Configurable to be either a trigger or gated
   Like a CD4017

   External pin 1       = Reset (not used)
   External pin 2 (PB3) = input 0 freq 1
   External pin 3 (PB4) = output 3
   External pin 4       = GND
   External pin 5 (PB0) = output 0 output
   External pin 6 (PB1) = output 1
   External pin 7 (PB2) = output 2
   External pin 8       = Vcc
   V 1.0  -  First Version
   Rob Stave (Rob the fiddler) ccby 2021
*/

//  ATTiny overview
//                      +-\/-+
//               Reset 1|    |8  VCC
// (pin3)          PB3 2|    |7  PB2 (pin2) out 2
// (pin4) in 1 A2  PB4 3|    |6  PB1 (pin1) out 1
//                 GND 4|    |5  PB0 (pin0) out 0
//                      ------


// Configurable settings

// set to true for a trigger, false for basic binary clock
const  bool triggerEnabled = true;

// Ranges for the pot.  Technically a small number means a
// shorter timer so low or high...whatever you want to call it.
// So if the longest time is not long enough for ya, change 2000 to something bigger.

#define CLOCK_COUNT_HIGH 170
#define CLOCK_COUNT_LOW 2000

// Length of the trigger.  This is not in ms, just clock ticks really.  Make this smaller than
// CLOCK_COUNT_HIGH.  It will be consistant, so you can tweak as needed
#define TRIGGER_COUNT_HIGH 100 // this should be smaller than CLOCK_COUNT_HIGH

// --------------------------------


// variables

volatile uint16_t oscCounter1 = 0;
volatile uint16_t count = 0;

// State of the clock
volatile byte clockCounter = 0;

// the setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00001111;  //set output bits

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
  if (oscCounter1 >= count) {
    oscCounter1 = 0;


    if (clockCounter == 0) {
      PORTB  = B00000001;
    }
    if (clockCounter == 1) {
      PORTB  = B00000010;
    }
    if (clockCounter == 2) {
      PORTB  = B00000100;
    }
    if (clockCounter == 3) {
      PORTB  = B00001000;
    }

    clockCounter = clockCounter + 1;
    if ( clockCounter >= 4 ) {
      clockCounter = 0;
    }


  }

  // if we are doing triggers, clear the port when you hit the trigger count
  if ((triggerEnabled == true) && (oscCounter1 == TRIGGER_COUNT_HIGH)) {
    PORTB  = B00000000;
  }

  oscCounter1++;
}

void loop() {

  
  while (true) {
    int osc1_t = analogRead(A2);
    count = map(osc1_t, 0, 1023, CLOCK_COUNT_LOW,  CLOCK_COUNT_HIGH);
  }
}
