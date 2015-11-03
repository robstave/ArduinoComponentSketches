
/**
 * ACS-85-0011
 * ATTiny85  Random Value LFO
 *
 * LFO that outputs a value randomly.
 * Kinda like psycho lfo...but simpler
 *
 *
 *
 *
 * Rob Stave (Rob the fiddler) ccby 2015
 */



//  ATTiny overview
//                           +-\/-+
//                    Reset 1|    |8  VCC
//     (pin3) Speed A3  PB3 2|    |7  PB2 (pin2) none
//      (pin4) none A2  PB4 3|    |6  PB1 (pin1) Digital Square out
//                      GND 4|    |5  PB0 (pin0) PWM LFO out
//                           ------



unsigned int lfsr  = 1;

#define LFO_LOW 100
#define LFO_HIGH 12

//Counter. Start at 255 so it resets on first loop
int loopCount = 255;

//speed
int oscFreq1 = 50;
int oscCounter1 = 0;



void clockLfsr () {

  //calculate new state
  boolean outputBit = bitRead(lfsr, 10) ^ bitRead(lfsr, 12)
                      ^ bitRead(lfsr, 13) ^ bitRead(lfsr, 15);
  lfsr = lfsr << 1;
  lfsr |= outputBit;
}

// the setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00000111;  //set output bits

 
  noInterrupts();           // disable all interrupts

 // initialize timer0
   // Timer 0, A side
  TCCR0A = _BV (WGM00) | _BV (WGM01) | _BV (COM0A1); // fast PWM, clear OC0A on compare
  TCCR0B = _BV (CS00);           // fast PWM, top at 0xFF, no prescaler

  // Initial value for our pulse width is 0
  OCR0A = 0x00;

 // initialize timer1
  TCCR1 = 0;                  //stop the timer
  TCNT1 = 0;                  //zero the timer
  //GTCCR = _BV(PSR1);          //reset the prescaler
  OCR1A = 100;                //set the compare value
  OCR1C = 100;

  TIMSK |= (1 << OCIE1A); //interrupt on Compare Match A  /works with timer
  //TIMSK = _BV(OCIE1A);        //interrupt on Compare Match A

  TCCR1 = _BV(CTC1) | _BV(CS10) | _BV(CS11); // Start timer, ctc mode, prescaler clk/2

  interrupts();             // enable all interrupts

}



ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{

  //Count up and toggle portB bits
  if (oscCounter1 > oscFreq1) {
    oscCounter1 = 0;

    clockLfsr();

    loopCount++;
    if (loopCount > 255) {
      loopCount = 0;
    }

    //Squarewave output
    if (loopCount > 127) {
      PORTB &= B11111101;
    } else {
      PORTB |= B00000010;
    }

    if (loopCount == 0) {
      setRandomValue();
    }


  }
  oscCounter1++;


}


void setRandomValue() {
  byte reg = lfsr & B11111111;
  OCR0A = reg;
}


void loop() {
  int speed_read = analogRead(A3);
  oscFreq1 = map(speed_read, 0, 1023, LFO_LOW,  LFO_HIGH);
}



