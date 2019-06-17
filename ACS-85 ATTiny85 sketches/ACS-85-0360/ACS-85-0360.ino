/**
   ACS-85-0360
   Counter with divide selection

   Clocks from the PB2 and divides by 8
   So you need an external clock.  Can be used as a LFO or
   frequency counter.

   Switches enable lower multiples so pressing
   PB3 will divide by 4 (making it sound like it is going faster)
   PB4 will divied by 2
   PB4 and PB3 will let the clock pass.

   After a low frequency, you can use as an LFO with a stutter.

   External pin 1       = Reset (not used)
   External pin 2 (PB3) = div 2 switch
   External pin 3 (PB4) = div 4 switch (both high for div 8)
   External pin 4       = GND
   External pin 5 (PB0) = Out
   External pin 6 (PB1) = gate/trigger config
   External pin 7 (PB2) = Clock
   External pin 8       = Vcc

   V 1.0  -  First Version

   Note: This sketch has been written specifically for ATTINY85 and not Arduino uno
   Rob Stave (Rob the fiddler) CCBY 2018
*/


//  ATTiny overview
//                          +-\/-+
//                   Reset 1|    |8  VCC
// (pin3) div 2 switch PB3 2|    |7  PB2 (pin2/int0) CLOCK
// (pin4) div 4 switch PB4 3|    |6  PB1 (pin1) gate
//                     GND 4|    |5  PB0 (pin0) out
//                          ------


const int int0 = 0;  // interrupt 0 - Clock interrupt
const byte LFOPIN = PB0;

volatile  uint8_t clockCount = 0;

volatile boolean useTrigger = false;
volatile boolean mult2 = false;
volatile boolean mult4 = false;


void setup() {
  // put your setup code here, to run once:
  DDRB = B00000001; // set PORTB

  //  clockCounter is our interrupt, function is called when
  //  invoked on either clock edge
  attachInterrupt(int0, clockCounter, CHANGE);

}


/**
   Simple divide
*/
void outputAsDiv(byte cc) {

  byte bitToRead = 3;

  if (mult2 == true && mult4 == true) {
    bitToRead = 0;
  } else if (mult2 == true) {
    bitToRead = 2;
  } else if (mult4 == true) {
    bitToRead = 1;
  }

  if ( bitRead(cc, bitToRead)) {
    PORTB |= (_BV(LFOPIN));
  } else {
    PORTB &= ~_BV(LFOPIN);
  }
}

/**
   Divide...but only output counts as triggers, not 50% duty
*/
void outputAsTrigger (byte cc) {

  if ((mult2 == true) && (mult4 == true)) {
    if ( bitRead(cc, 0)) {
      PORTB |= (_BV(LFOPIN));
    } else {
      PORTB &= ~_BV(LFOPIN);
    }
    return;

  } else if (mult4 == true) {

    if ( (cc & B00000011) == B00000011) {
      PORTB |= (_BV(LFOPIN));
    } else {
      PORTB &= ~_BV(LFOPIN);
    }
    return;


  } else if (mult2 == true) {
    if ( (cc & B00000111) == B00000111) {
      PORTB |= (_BV(LFOPIN));
    } else {
      PORTB &= ~_BV(LFOPIN);
    }
    return;
  } else {

    if ( (cc & B00001111) == B00001111) {
      PORTB |= (_BV(LFOPIN));
    } else {
      PORTB &= ~_BV(LFOPIN);
    }
    return;

  }

}

void clockCounter()      // called by interrupt
{
  clockCount--;

  if (useTrigger == true) {
    outputAsTrigger(clockCount);
  } else {
    outputAsDiv(clockCount);
  }
}


void loop() {
  // put your main code here, to run repeatedly:

  byte inputPortB = PINB;

  useTrigger =  bitRead(inputPortB, 1);
  mult2 =  bitRead(inputPortB, 3);
  mult4 =  bitRead(inputPortB, 4);

}
