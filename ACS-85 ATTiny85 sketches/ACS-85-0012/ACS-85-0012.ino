
/**
 * ACS-85-0011
 * ATTiny85  Random Value LFO 2
 *
 * Not super amazing...but fun to play with for a bit.
 * On the random LFO, there is generally a slew/glide.
 * This is done by changing the value of a LP filter and
 * making the voltage work its way to the new state.  The glide
 * rate is determined by the rc constant.
 *
 * In this case, we are averaging our way to the next value over the
 * course of several steps.
 *
 * Rob Stave (Rob the fiddler) ccby 2015
 */



//  ATTiny overview
//                           +-\/-+
//                    Reset 1|    |8  VCC
//     (pin3) Speed A3  PB3 2|    |7  PB2 (pin2) nc
//      (pin4) Glide A2  PB4 3|    |6  PB1 (pin1) Digital Square out
//                      GND 4|    |5  PB0 (pin0) PWM LFO out
//                           ------



unsigned int lfsr  = 1;

#define LFO_LOW 100
#define LFO_HIGH 3


int lastValue = 0;
int newValue = 0;

int glideCount = 0;
int glide = 1;


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

  DDRB = B00000011;  //set output bits


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
  OCR1A = 20;                //set the compare value
  OCR1C = 20;

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

  if (glideCount > glide) {
    glideCount = 0;
    newValue = lfsr & B11111111;

  }
  glideCount++;

  byte x = (lastValue >> 1) + (newValue >> 1);
  lastValue = x;
  OCR0A = lastValue;
}



int readCount = 1;

void loop() {
  //Read the Speed
  int speed_read = analogRead(A3);
  oscFreq1 = map(speed_read, 0, 1023, LFO_LOW,  LFO_HIGH);


  //Rather spend more time reading the speed than that glides
  if (readCount > 20) {
    readCount = 0;

    //read the glide
    int glide_read = analogRead(A2);
    glide = map(glide_read, 0, 1023, 2,  13);

  }
  readCount++;


}



