/**
   ACS-85-0201

   Simple VCA and Envelope

   Takes a squarewave input and a generated envelope.
   There is really only one paramter.  Attack and release are the same
   parameter, although release is twice as long.

   Envelope is triggered by the trigger pin and there is a pin to
   set the attack to instant.

   External pin 1       = Reset (not used)
   External pin 2 (PB3) = Attack
   External pin 3 (PB4) = output (PWM)
   External pin 4       = GND
   External pin 5 (PB0) = Attack Enabled
   External pin 6 (PB1) = Trigger
   External pin 7 (PB2) = Clock
   External pin 8       = Vcc

   V 1.0  -  First Version

   Improvements:
    - maybe make the map log-ish
    - Im not reading the value of the clock pin, just flippling so it may not be
      super good for LFO cases.  We should sample a few times first I suppose
      at a minimum or maybe every time.

   Note: This sketch has been written specifically for ATTINY85 and not Arduino uno,
      but should not be hard to fix it.

   Rob Stave (Rob the fiddler) CCBY 2018


   ideas  set volume = 255 immediately! if attack is out

   get rid of ints make bytes and use max
*/


//               ATTiny85 overview
//                     +-\/-+
//              Reset 1|    |8  VCC
// (pin3)  volume PB3 2|    |7  PB2 (pin2/int0) CLOCK
// (pin4)out(pwm) PB4 3|    |6  PB1 (pin1) trigger
//                GND 4|    |5  PB0 (pin0) Attack Active
//                     ------


const int int0 = 0; // interrupt 0

volatile uint8_t volume;
volatile uint8_t state = HIGH; // State of the squarewave (we just flip a bit on change pin)


enum ASDR_STATES {
  NONE,
  ATTACK,
  DELAY,
  SUSTAIN,
  RELEASE
};

volatile ASDR_STATES arState  = NONE;

volatile uint8_t attackSteps = 100;
volatile uint16_t attackCounter = 0;

volatile uint8_t releaseSteps = 100;
volatile uint16_t releaseCounter = 0;




// the setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00010000;  //set output bits

  // initialize timers
  noInterrupts();           // disable all interrupts
  // Enable 64 MHz PLL and use as source for Timer1
  PLLCSR = 1 << PCKE | 1 << PLLE;

  // Set up Timer/Counter1 for PWM output
  TIMSK = 0;                     // Timer interrupts OFF
  TCCR1 = 1 << CS10;             // 1:1 prescale
  GTCCR = 1 << PWM1B | 2 << COM1B0; // PWM B, clear on match

  // Set up Timer/Counter0 for 100ish-Hz interrupt to output samples.
  TCCR0A = 3 << WGM00;           // Fast PWM
  TCCR0B = 1 << WGM02 | 5 << CS00; // 1/1024 prescale
  TIMSK = 1 << OCIE0A;           // Enable compare match, disable overflow
  OCR0A = 79;                    // Divide by 1024*80

  interrupts(); // enable all interrupts

  //  clockCounter function is called when invoked on changing  clock edge
  attachInterrupt(int0, clockCounter, CHANGE);

}

ISR(TIMER0_COMPA_vect) {


  if (arState == ATTACK) {

    attackCounter = attackCounter + attackSteps;

    if ( attackCounter < 255) {
      volume = char(attackCounter);
    } else {
      arState = RELEASE;
      volume = 255;
      releaseCounter = 0;
    }

    return;

  } else if (arState == RELEASE) {

    releaseCounter = releaseCounter + releaseSteps;

    if ( releaseCounter < 255) {
      volume = 255 - char(releaseCounter);
    } else {

      arState = NONE;
      releaseCounter = 0;
      attackCounter = 0;
      volume = 0;
    }
    return;

  } else {
    volume = 0;
  }
}



void clockCounter()
{
  // Called by pin interrupt.  We are effectively chopping the signal.
  // If the input is high, we set the volume to the envelope, otherwise zero.
  // We could read the value of the pin too if you were doing this for an LFO kinda thing.

  state = !state;
  if (state == HIGH) {
    OCR1B  = volume;
  } else {
    OCR1B  = 0;
  }
}

boolean isTriggerStarted() {
  // Check pin 1
  return ((PINB & 0b00000010) != 0);
}

boolean checkAttackEnabled() {
  // Check pin 0
  return ((PINB & 0b00000001) != 0);
}


void startEnvelope() {
  releaseCounter = 0;
  attackCounter = 0;
  arState = ATTACK;

  // As a bonus to get things running fast, lets set the volume to
  // max so we are not waiting an envelope cycle
  if (attackSteps == 255){
      volume = 255;
  }

}


void loop() {

  byte count = 0;
  boolean isTriggerDebounce = false;
  boolean isAttackEnabled = true;

  while (true) {

    count++;

    // This is set up this way so I only read A3 every so often
    // It will make the button a little more responsive perhaps.
    // Maybe not so much in this case, but Im using the analog read
    // as my debounce delay as well.

    if (count == 3) {

      isAttackEnabled = checkAttackEnabled();


      int sample = analogRead(A3);
      int stepCount = map(sample, 0, 1023, 100, 1);

      if ( isAttackEnabled == true) {

        attackSteps = stepCount;
        //Make delay twice as long as attack
        releaseSteps = max(attackSteps >> 1, 1);

      } else {
        attackSteps = 255;
        releaseSteps = stepCount;


      }

      // We will debounce here as well
      if ( isTriggerDebounce &&  !isTriggerStarted()) {
        isTriggerDebounce = false;
      }
    }

    if (arState == NONE) {
      if ( !isTriggerDebounce &&  isTriggerStarted()) {
        isTriggerDebounce = true;
        startEnvelope();
        count = 0;
      }
    }

    if (count > 5) {
      count = 0;
    }
  }

}