

/**
 * ACS-85-0500
 * ATTiny85 Atari punk console
 *
 * This is a pretty close approximation to the
 * APC.  It consists of two components.
 * The first is an oscillator.
 * when the oscillator changes down, it triggers a
 * one shot.  When the one shot is finished, the first oscilllator is
 * free to trigger the one shot again.
 *
 * without touching the knobs, its just a squarewave.  As you modify
 * the frequency or one shot length, you get a PWM/Aliasing effect.
 *
 * Alas, the resolution as it stands makes an already underwhelming
 * thing into an 8-bit version that is more underwhelming.
 *
 * it works..but could be improved.

 *
 * External pin 1       = Reset (not used)
 * External pin 2 (PB3) = Osc freq
 * External pin 3 (PB4) = one shot length
 * External pin 4       = GND
 * External pin 5 (PB0) = none
 * External pin 6 (PB1) = none
 * External pin 7 (PB2) = out
 * External pin 8       = Vcc

 *
 * V 1.0  -  First Version
 *   needs work. Tried to limit analogReads with millis, but that did not work.
 *   maybe there is a conflict w timer 0.

 *
 * Rob Stave (Rob the fiddler) ccby 2015
 */

//  ATTiny overview
//                        +-\/-+
//                 Reset 1|    |8  VCC
//       (pin3) OSC  PB3 2|    |7  PB2 out
//  (pin4) ONE SHOT  PB4 3|    |6  PB1 nc
//                   GND 4|    |5  PB0 nc
//                        ------

// counters for the frequencies
int oscFreq = 55;
int oscCounter = 0;

int oneShotLength = 22;
int oneShotCounter = 0;

bool oneShotReady = true;
bool oneShotActive = false;

bool osc = false;

// avoid a constant call to analog read
long readTimer = 0;

// the setup function runs once when you press reset or power the board
void setup()
{

  DDRB = B00000111; // could use this

  // initialize timer0
  noInterrupts(); // disable all interrupts

  TCCR0A = 0;
  TCCR0B = 0;

  TCCR0A |= (1 << WGM01); // Start timer 1 in CTC mode Table 11.5
  TIMSK |= (1 << OCIE0A); // Enable CTC interrupt see 13.3.6
  OCR0A = 4;              // CTC Compare value

  TCCR0B |= (1 << CS00); // Prescaler =64 Table 11.6

  interrupts(); // enable all interrupts
}

ISR(TIMER0_COMPA_vect) // timer compare interrupt service routine
{

  readTimer++;
  // HF code is the same.  Just smaller counters
  if (oscCounter > oscFreq)
  {
    oscCounter = 0;
    osc = !osc;

    if (oneShotReady && (osc == false))
    {
      oneShotReady = false;
      oneShotActive = true;
      oneShotCounter = 0;
    }
  }
  oscCounter++;

  if (oneShotActive == true)
  {
    PORTB |= (1 << PB2); // Set port High

    if (oneShotCounter > oneShotLength)
    {
      oneShotCounter = 0;
      PORTB &= ~(1 << PB2); // Set port Low
      oneShotReady = true;
      oneShotActive = false;
    }
    oneShotCounter++;
  }
}

void loop()
{

  if (readTimer > 500)
  {

    int osc1_t = analogRead(A2);
    oscFreq = map(osc1_t, 0, 1023, 40, 200);
    int oneShotDuration_t = analogRead(A3);
    oneShotLength = map(oneShotDuration_t, 0, 1023, 20, 200);
    readTimer = 0;
  }
}
