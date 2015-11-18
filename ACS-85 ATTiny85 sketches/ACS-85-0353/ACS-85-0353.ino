

/**
 * ACS-85-0353
 * ATTiny85 sample and modulate by LFSR
 *
 * Samples the squarewave on pin 7 and output
 * a squarewave that is a little higher and lower in frequency
 *
 *
 * External pin 1       = Reset (not used)
 * External pin 2 (PB3) = freq
 * External pin 3 (PB4) = width
 * External pin 4       = GND
 * External pin 5 (PB0) = freq - LFSR
 * External pin 6 (PB1) = freq + LFSR
 * External pin 7 (PB2) = Clock
 * External pin 8       = Vcc

 *
 * V 1.0  -  First Version
 *
 * Note: This sketch has been written specifically for ATTINY85 and not Arduino uno
 * You can prob adapt pretty easy though
 *
 * Rob Stave (Rob the fiddler) CCBY 2015
 */


//  ATTiny overview
//                          +-\/-+
//                   Reset 1|    |8  VCC
//       (pin3) width  PB3 2|    |7  PB2 (pin2/int0) CLOCK
//       (pin4) speed  PB4 3|    |6  PB1 (pin1) freq + LFSR
//                     GND 4|    |5  PB0 (pin0) freq - LFSR
//                          ------


#define SPEED_LOW  1200
#define SPEED_HIGH 40

const int int0 = 0;  // interrupt 0


volatile int oscFreq1 = 200;
volatile int  oscCounter1 = 0;

volatile int oscFreq2 = 200;
volatile int  oscCounter2 = 0;

unsigned long counter = 0;
unsigned long lastCounter = 0;
unsigned long diff = 0;



int loopSpeed = 20;
int detune_value = 20;




//Byte used as the LFSR
unsigned int lfsr  = 1;


void setup()
{

  // initialize timer1
  noInterrupts();           // disable all interrupts

  TCCR1 = 0;                  //stop the timer
  TCNT1 = 0;                  //zero the timer
  //GTCCR = _BV(PSR1);          //reset the prescaler
  OCR1A = 99;                //set the compare value
  OCR1C = 99;
  TCCR1 = _BV(CTC1) | _BV(CS10); // Start timer, ctc mode, prescaler clk/1
  TIMSK |= (1 << OCIE1A); //interrupt on Compare Match A  /works with timer

  interrupts();             // enable all interrupts


  DDRB = B00000011; // set PORTB to outputs except PB2 for the clock

  attachInterrupt(int0, clockCounter, CHANGE);
  //  clockInt is our interrupt, clockCounter function is called when
  //  invoked on either clock edge

}


/**
 * Sample the counter and store off the difference.
 * Clock LFSR as well
 */
void clockCounter()      // called by interrupt
{

  diff =  (diff + (counter - lastCounter)) >> 1;
  lastCounter = counter;
  clockLfsr ();

}



ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  //clock main counter
  counter++;


  //Count up and toggle portB bits
  if (oscCounter1 > oscFreq1) {
    oscCounter1 = 0;
    PORTB ^= (_BV(PB0));
  }
  oscCounter1++;


  //Count up and toggle portB bits
  if (oscCounter2 > oscFreq2) {
    oscCounter2 = 0;
    PORTB ^= (_BV(PB1));
  }
  oscCounter2++;

}



/**
 * Increment the LFSR for a new value
 */
void clockLfsr () {

  //calculate new state
  boolean outputBit = bitRead(lfsr, 10) ^ bitRead(lfsr, 12)
                      ^ bitRead(lfsr, 13) ^ bitRead(lfsr, 15);
  lfsr = lfsr << 1;
  lfsr |= outputBit;
}

/**
 * Get offset based on the detune value.
 * A high value gives more bits
 */
byte getOffset() {

  //3 bits  (0-7)
  if (detune_value < 33) {
    return lfsr & B00000111;
  }

  //4 bits (0-15)
  if (detune_value < 66) {
    return lfsr & B00001111;
  }

  //5 bits (0-32)
  if (detune_value < 104) {
    return lfsr & B00011111;
  }
}

void loop()
{

  //No need to always sample.
  //If the width or speed seems less responsive, drop this number

  int sample = analogRead(A3);
  detune_value = map(sample, 0, 1023, 0 ,  100);
  sample = analogRead(A2);
  loopSpeed = map(sample, 0, 1023, SPEED_HIGH,  SPEED_LOW);



  delay(loopSpeed);

  oscFreq1 = diff + getOffset();
  oscFreq2 = diff - getOffset();


}
