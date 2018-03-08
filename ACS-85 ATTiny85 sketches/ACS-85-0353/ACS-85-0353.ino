

/**
   ACS-85-0353
   ATTiny85 sample and modulate by LFSR

   Samples the squarewave on pin 7 and output
   a squarewave that is a little higher and lower in frequency


   External pin 1       = Reset (not used)
   External pin 2 (PB3) = freq
   External pin 3 (PB4) = width
   External pin 4       = GND
   External pin 5 (PB0) = freq - LFSR
   External pin 6 (PB1) = freq + LFSR
   External pin 7 (PB2) = Clock
   External pin 8       = Vcc


   V 1.0  -  First Version
   V 1.1  -  Rewrote with own millis

   Note: This sketch has been written specifically for ATTINY85 and not Arduino uno
   You can prob adapt pretty easy though

   Rob Stave (Rob the fiddler) CCBY 2015
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


volatile  unsigned int oscFreq1 = 200;
volatile  unsigned int oscCounter1 = 0;

volatile  unsigned int oscFreq2 = 200;
volatile  unsigned int oscCounter2 = 0;

volatile unsigned int counter = 0;
volatile unsigned int lastCounter = 0;
volatile unsigned int diff = 0;

//Byte used as the LFSR
volatile unsigned int lfsr  = 1;

void setup()
{

  // Initialize ports

  DDRB = B00000011; // set PORTB so that pin 0 and 1 are outputs

  // initialize timer1
  noInterrupts();           // disable all interrupts


  TCCR0A = 0;
  TCCR0B = 0;

  TCCR0A |= (1 << WGM01); //Start timer 1 in CTC mode Table 11.5
  OCR0A = 145; //CTC Compare value
  TCCR0B |= (1 << CS00); // Prescaler =8 Table 11.6

  // TCCR0A |=(1<<COM0A1); //Timer0 in toggle mode Table 11.2
  TIMSK |= (1 << OCIE0A); //Enable CTC interrupt see 13.3.6



  // start the timer, prescaler
  TCCR1 = (1 << CTC1) | (7 << CS10); // CTC  mode, div64
  OCR1C = 0.001 * F_CPU / 64 - 1; // 1ms, F_CPU @16MHz, div64
  TIMSK |= (1 << OCIE1A);



  //  clockInt is our interrupt, clockCounter function is called when
  //  invoked on either clock
  // Set it to INT0 (for ATTINY85)


  interrupts();             // enable all interrupts

  attachInterrupt(0, clockCounter, CHANGE);


}

volatile unsigned long milliseconds;

ISR(TIMER1_COMPA_vect)
{
  milliseconds++;
}

unsigned long millisTimer1()
{
  return milliseconds;
}


/**
   Sample the counter and store off the difference.
   Clock LFSR as well
*/
void clockCounter()      // called by interrupt
{

  diff =  (diff + (counter - lastCounter)) / 2 ;
  lastCounter = counter;
}



ISR(TIMER0_COMPA_vect)          // timer compare interrupt service routine
{
  //clock main counter
  counter++;

  //OUTPUT ->
  //Count up and toggle portB bits
  if (oscCounter1 <= 0 ) {
    oscCounter1 = oscFreq1;

    //Toggle PB0
    PORTB ^= (_BV(PB0));
  }
  oscCounter1--;

  //OUTPUT ->
  //Count up and toggle portB bits
  if (oscCounter2 <= 0 ) {
    oscCounter2 = oscFreq2;

    //Toggle PB1
    PORTB ^= (_BV(PB1));
  }
  oscCounter2--;

}



/**
   Increment the LFSR for a new value
*/
void clockLfsr () {

  //calculate new state
  boolean outputBit = bitRead(lfsr, 10) ^ bitRead(lfsr, 12)
                      ^ bitRead(lfsr, 13) ^ bitRead(lfsr, 15);
  lfsr = lfsr << 1;
  lfsr |= outputBit;
}

/**
   Get offset based on the detune value.
   A high value gives more bits
*/
byte getOffset(byte v) {



  //3 bits  (0-7)
  if (v < 33) {
    return lfsr & B00000111;
  }

  //4 bits (0-15)
  if (v < 66) {
    return lfsr & B00001111;
  }

  //5 bits (0-32)
  if (v < 104) {
    return lfsr & B00011111;
  }
}

 
void loop()
{

  int loopSpeed = 20;
  unsigned long previousMillis = 0;        // will store last time LED was updated

  clockLfsr();
  while (true) {

    int sample = analogRead(A2);
    loopSpeed = map(sample, 0, 1023, SPEED_HIGH,  SPEED_LOW);

    unsigned long currentMillis = millisTimer1();
    if (currentMillis - previousMillis >= loopSpeed) {
      // save the last time you blinked the LED
      previousMillis = currentMillis;

      int sample2 = analogRead(A3);
      byte detune_value = map(sample2, 0, 1023, 0 ,  100);
      oscFreq1 = diff + getOffset(detune_value );
      oscFreq2 = diff - getOffset(detune_value);

    }
    clockLfsr();


  }

}