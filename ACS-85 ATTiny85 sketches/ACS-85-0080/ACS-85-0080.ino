/**
   ACS-85-0080 ATTiny85 - noise
   External pin 2 = channel 1 gate
   External pin 3 = input
   External pin 5 = out 1
   External pin 6 = out 2
   External pin 7 = channel 2 gate
    ATTiny overview
                            +-\/-+
                     Reset 1|    |8  VCC
    (pin3) chan1 gate  PB3 2|    |7  PB2 channel 2 gate
           (pin4) freq PB4 3|    |6  PB1 (pin1)  out 2
                       GND 4|    |5  PB0 (pin0)  out 1
                            ------
   Description:
   two noise channels.  One input to tune,
   V 1.0  -  First Version
   Rob Stave (Rob the Fiddler) CCBY 2021
*/


// These values make noise 1 a higher pitch that noise 2.
// if you want to spread the frequency...change the values.
// in this case..the 50 is the highest sound ( 50 ticks per LFSR tick) and 
// 180 is the lower range.  
#define CHANNEL1_HIGH 50
#define CHANNEL1_LOW 180

// This is for the second channel.
// if your driving the analog read with a cv...you can flip the high
// and low too so one goes up and the other goes down.
#define CHANNEL2_HIGH 90
#define CHANNEL2_LOW 250

volatile unsigned int lfsr1 = 1;
volatile unsigned int lfsr2 = 3;


volatile uint16_t oscCounter1 = 0;
volatile uint16_t count1 = 0;
volatile uint16_t oscCounter2 = 0;
volatile uint16_t count2 = 0;

volatile boolean outputBit1  = false;
volatile boolean outputBit2   = false ;

// the setup function runs once when you press reset or power the board

void setup() {

  DDRB = B00000011;  //set output bits

  // initialize timer1
  noInterrupts();           // disable all interrupts

  TCCR1 = 0;                  //stop the timer
  TCNT1 = 0;                  //zero the timer
  //GTCCR = _BV(PSR1);          //reset the prescaler
  OCR1A = 30;                //set the compare value
  OCR1C = 30;
  TIMSK = _BV(OCIE1A);        //interrupt on Compare Match A

  TCCR1 = _BV(CTC1)  | _BV(CS11) ; // Start timer, ctc mode, prescaler clk/8

  interrupts();             // enable all interrupts
}

// Used for the the LFSR(noise)
void clockLfsr1 () {

  //calculate new state
  boolean outputBit = bitRead(lfsr1, 10) ^ bitRead(lfsr1, 12)
                      ^ bitRead(lfsr1, 13) ^ bitRead(lfsr1, 15);
  lfsr1 = lfsr1 << 1;
  lfsr1 |= outputBit;
}

void clockLfsr2 () {

  //calculate new state
  boolean outputBit = bitRead(lfsr2, 10) ^ bitRead(lfsr2, 12)
                      ^ bitRead(lfsr2, 13) ^ bitRead(lfsr2, 15);
  lfsr2 = lfsr2 << 1;
  lfsr2 |= outputBit;
}


ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  if (oscCounter1 >= count1) {
    oscCounter1 = 0;
    clockLfsr1();
    if (outputBit1 &&   bitRead(lfsr1, 1)) {
      bitSet(PORTB, 0);
    } else {
      bitClear(PORTB, 0);
    }
  }
  oscCounter1++;

  if (oscCounter2 >= count2) {
    oscCounter2 = 0;
    clockLfsr2();
    if (outputBit2 &&  bitRead(lfsr2, 1)) {
      bitSet(PORTB, 1);
    } else {
      bitClear(PORTB, 1);
    }
  }
  oscCounter2++;
}

void loop()
{
   
  byte readCount = 0;

  while (true) {

    // no need to read pots every loop?
    if ( readCount % 3 == 0 ) {
      int osc1_t = analogRead(A2);
      count1 = map(osc1_t, 0, 1023, CHANNEL1_LOW,  CHANNEL1_HIGH);
      count2 = map(osc1_t, 0, 1023, CHANNEL2_LOW,  CHANNEL2_HIGH);
    }
    
    byte inputPortB = PINB;

    outputBit1 = bitRead(inputPortB, 2);
    outputBit2 = bitRead(inputPortB, 3);

    readCount++;
  }
}
