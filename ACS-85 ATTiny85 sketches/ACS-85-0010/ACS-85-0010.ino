
/**
 * ACS-85-0010
 * ATTiny85  Selectable waveform LFO
 * Select between triangle, ramp up ramp down and sine
 *
 * The square wave is just checking that OC0a is greater or less than 127.
 *
 * The remaining pin kinda shows an odd/even of the state. Useful for debugging.
 *
 * The values are being set in loop, so you can not assume that its working super
 * precise.  Generally, I would sample things like the wave shape maybe once a second
 * or so, but if we did that in this case, we might see staggers in the output.
 *
 * Notes:
 * 
 * Im not getting fastpwm  the pwm is still 500hz?
 * 
 * 
 * When using an arduino analog pin as a select input, you should use a linear pot.
 * That way, if you are selecting between 4 states, like here, the range is evenly
 * distributed.  Sometimes, you do not have that available.  
 * 
 * Not a problem.
 * 
 * I find that mapping the analog to a small range like 0-3 does not work too well.
 * First off, I map it to a bigger range and use if statements. So like 0-40 and check
 * on divisors of 10.
 *
 * if you have a linear pot though, you need to tweek the values. MAP_VALUES_AS_LINEAR
 * lets us define a different mapping.  Much better for this case.
 *
 *
 *
 *
 * Rob Stave (Rob the fiddler) ccby 2015
 */



//  ATTiny overview
//                           +-\/-+
//                    Reset 1|    |8  VCC
//     (pin3) Speed A3  PB3 2|    |7  PB2 (pin2) indicator
//    (pin4) Select A2  PB4 3|    |6  PB1 (pin1) Digital Square out
//                      GND 4|    |5  PB0 (pin0) PWM LFO out
//                           ------

uint8_t  sine_wave[256] = {
  0x80, 0x83, 0x86, 0x89, 0x8C, 0x90, 0x93, 0x96,
  0x99, 0x9C, 0x9F, 0xA2, 0xA5, 0xA8, 0xAB, 0xAE,
  0xB1, 0xB3, 0xB6, 0xB9, 0xBC, 0xBF, 0xC1, 0xC4,
  0xC7, 0xC9, 0xCC, 0xCE, 0xD1, 0xD3, 0xD5, 0xD8,
  0xDA, 0xDC, 0xDE, 0xE0, 0xE2, 0xE4, 0xE6, 0xE8,
  0xEA, 0xEB, 0xED, 0xEF, 0xF0, 0xF1, 0xF3, 0xF4,
  0xF5, 0xF6, 0xF8, 0xF9, 0xFA, 0xFA, 0xFB, 0xFC,
  0xFD, 0xFD, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFE, 0xFD,
  0xFD, 0xFC, 0xFB, 0xFA, 0xFA, 0xF9, 0xF8, 0xF6,
  0xF5, 0xF4, 0xF3, 0xF1, 0xF0, 0xEF, 0xED, 0xEB,
  0xEA, 0xE8, 0xE6, 0xE4, 0xE2, 0xE0, 0xDE, 0xDC,
  0xDA, 0xD8, 0xD5, 0xD3, 0xD1, 0xCE, 0xCC, 0xC9,
  0xC7, 0xC4, 0xC1, 0xBF, 0xBC, 0xB9, 0xB6, 0xB3,
  0xB1, 0xAE, 0xAB, 0xA8, 0xA5, 0xA2, 0x9F, 0x9C,
  0x99, 0x96, 0x93, 0x90, 0x8C, 0x89, 0x86, 0x83,
  0x80, 0x7D, 0x7A, 0x77, 0x74, 0x70, 0x6D, 0x6A,
  0x67, 0x64, 0x61, 0x5E, 0x5B, 0x58, 0x55, 0x52,
  0x4F, 0x4D, 0x4A, 0x47, 0x44, 0x41, 0x3F, 0x3C,
  0x39, 0x37, 0x34, 0x32, 0x2F, 0x2D, 0x2B, 0x28,
  0x26, 0x24, 0x22, 0x20, 0x1E, 0x1C, 0x1A, 0x18,
  0x16, 0x15, 0x13, 0x11, 0x10, 0x0F, 0x0D, 0x0C,
  0x0B, 0x0A, 0x08, 0x07, 0x06, 0x06, 0x05, 0x04,
  0x03, 0x03, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x03,
  0x03, 0x04, 0x05, 0x06, 0x06, 0x07, 0x08, 0x0A,
  0x0B, 0x0C, 0x0D, 0x0F, 0x10, 0x11, 0x13, 0x15,
  0x16, 0x18, 0x1A, 0x1C, 0x1E, 0x20, 0x22, 0x24,
  0x26, 0x28, 0x2B, 0x2D, 0x2F, 0x32, 0x34, 0x37,
  0x39, 0x3C, 0x3F, 0x41, 0x44, 0x47, 0x4A, 0x4D,
  0x4F, 0x52, 0x55, 0x58, 0x5B, 0x5E, 0x61, 0x64,
  0x67, 0x6A, 0x6D, 0x70, 0x74, 0x77, 0x7A, 0x7D
};



int state = 0;


/**
 * Set 1 for a linear pot and something else for other.
 * Audio pots are a little different, so dividing the
 * numbers have problems
 */
#define MAP_VALUES_AS_LINEAR 1

#if MAP_VALUES_AS_LINEAR == 1
  #define STATE1 10
  #define STATE2 20
  #define STATE3 30
  #define STATE4 40
#else
  #define STATE1 5
  #define STATE2 10
  #define STATE3 20
  #define STATE4 40
#endif

#define LFO_LOW 200
#define LFO_HIGH 12

//used for the squarewave
int loopCount = 255;

//speed
int oscFreq1 = 50;
int oscCounter1 = 0;



// the setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00000111;  //set output bits

  // initialize timer0
  noInterrupts();           // disable all interrupts

  // Clear OC0A on match
  TCCR0A |= _BV(COM0A1);

  // Set timer to count with F_CPU / 64
  //TCCR0B |= _BV(CS10); // F_CPU
  //TCCR0B |= _BV(CS01) | _BV(CS00);  // /64 prescale

  // Use Fast PWM, OCRA TOP
  TCCR0A |= _BV(WGM00);
  TCCR0A |= _BV(WGM01);

  // Initial value for our pulse width is 0
  OCR0A = 0x00;



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


int dir = 1;

void doTriangle () {
  if (OCR0A >= 255 || OCR0A <= 0) {
    // Switch directon at top
    if (OCR0A == 255) {
      dir = -1;
    }  else { //
      dir = 1;
    }
  }
  OCR0A += dir;
}

void rampUp () {
  OCR0A += 1;
  if (OCR0A >= 255  ) {
    OCR0A = 0;
  }
}

void rampDown () {
  OCR0A -= 1;
  if ( OCR0A <= 0) {
    OCR0A = 255;
  }
}

void doSine() {
  OCR0A = sine_wave[loopCount];
}


ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{

  //Count up and toggle portB bits
  if (oscCounter1 > oscFreq1) {
    oscCounter1 = 0;

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


    //Figure out what the state is and display the
    //LFO output
    
    if (state <= STATE1) {
      doTriangle();
    } else if (state <= STATE2) {
      rampUp();
    } else if (state <= STATE3) {
      rampDown();
    } else if (state <= STATE4) {
      doSine();
    }
  }
  
  oscCounter1++;

}


/**
 * not super useful..just show an odd/even bit on PB2
 */
void showState () {
  if (state <= STATE1) {
    PORTB |= B00000100;
  } else if (state <= STATE2) {
    PORTB &= B11111011;
  } else if (state <= STATE3) {
    PORTB |= B00000100;
  } else if (state <= STATE4) {
    PORTB &= B11111011;
  }
}

void loop() {

  int speed_read = analogRead(A3);
  oscFreq1 = map(speed_read, 0, 1023, LFO_LOW,  LFO_HIGH);

  int shape_read = analogRead(A2);
  state = map(shape_read, 0, 1023, 0,  40);

  showState();

}



