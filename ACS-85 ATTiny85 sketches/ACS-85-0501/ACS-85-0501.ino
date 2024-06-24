

/**
 * ACS-85-0501
 * ATTiny85 Atari punk console
 *
 * This is a pretty close approximation to the
 * APC.  It consists of two components.
 * The first is an oscillator.
 * when the oscillator transitions down, it triggers a
 * one shot.  When the one shot is finished, the first oscilllator is
 * free to trigger the one shot again.
 *
 * without touching the knobs, its just a squarewave.  As you modify
 * the frequency or one shot length, you get a PWM/Aliasing effect.
 *
 * Alas, the resolution as it stands makes an already underwhelming
 * thing into an 8-bit version that is more underwhelming.
 *
 * it works..This is actually an improvement on 500.  But not by much.
 * I added some filtering on the inputs and added volitiles and moved pins around
 *  Added more output values too.

 *
 * External pin 1       = Reset (not used)
 * External pin 2 (PB3) = Osc freq
 * External pin 3 (PB4) = one shot length
 * External pin 4       = GND
 * External pin 5 (PB0) = out
 * External pin 6 (PB1) = out2
 * External pin 7 (PB2) = 
 * External pin 8       = Vcc

//  ATTiny overview
//                          +-\/-+
//                   Reset 1|    |8  VCC
// (pin3) Osc freq     PB3 2|    |7  PB2 (pin2/int0) nc
// (pin4) One shot len PB4 3|    |6  PB1 (pin1) out2
//                     GND 4|    |5  PB0 (pin0) out
//                          ------

 *
 * V 1.0  -  First Version
 *
 * Rob Stave (Rob the fiddler) ccby 2024
 */

 
// counters for the frequencies
volatile uint16_t oscFreq = 55;
volatile uint16_t oscCounter = 0;

volatile uint16_t oneShotLength = 22;
volatile uint16_t oneShotCounter = 0;

volatile bool oneShotArmed = true;
volatile bool oneShotActive = false;

volatile bool osc = false;

// the setup function runs once when you press reset or power the board
void setup()
{

  DDRB = B00000011; // could use this

  // initialize timer0
  noInterrupts(); // disable all interrupts

  TCCR0A = 0;
  TCCR0B = 0;

  TCCR0A |= (1 << WGM01); // Start timer 1 in CTC mode Table 11.5
  TIMSK |= (1 << OCIE0A); // Enable CTC interrupt see 13.3.6
  OCR0A = 2;              // CTC Compare value

  TCCR0B |= (1 << CS00); // Prescaler =64 Table 11.6

  interrupts(); // enable all interrupts
}

ISR(TIMER0_COMPA_vect) // timer compare interrupt service routine
{

  // HF code is the same.  Just smaller counters
  // Internally, this loop is always going.

  if (oscCounter > oscFreq)
  {
    oscCounter = 0;
    osc = !osc;
    // Toggle
    // If we are failling, trigger the one shot if it is armed.
    if (oneShotArmed && (osc == false))
    {
      oneShotArmed = false;
      oneShotActive = true;
      oneShotCounter = 0;
    }
  }
  oscCounter++;

  if (oneShotActive == true)
  {
    PORTB |= (1 << PB0); // Set port High

    if (oneShotCounter > oneShotLength)
    {
      oneShotCounter = 0;
      PORTB &= ~(1 << PB0); // Set port Low
      oneShotArmed = true;
      oneShotActive = false;
    }
    oneShotCounter++;

    if (oneShotCounter % 2 && osc)
    {
      PORTB |= (1 << PB1);
    }
    else
    {
      PORTB &= ~(1 << PB1); // Set port Low
    }
  }
}

void loop()
{

  byte counter = 0;

  // Used for sample averaging
  int f1Sample[4] = {0, 0, 0, 0};
  int f2Sample[4] = {0, 0, 0, 0};

  while (true)
  {

    f1Sample[counter] = analogRead(A2);
    unsigned int osc1_t = (f1Sample[0] + f1Sample[1] + f1Sample[2] + f1Sample[3]) >> 2;
    oscFreq = map(osc1_t, 0, 1023, 20, 420);

    f2Sample[counter] = analogRead(A3);
    unsigned int oneShotDuration_t = (f2Sample[0] + f2Sample[1] + f2Sample[2] + f2Sample[3]) >> 2;
    oneShotLength = map(oneShotDuration_t, 0, 1023, 10, 420);

    counter++;
    if (counter > 3)
    {
      counter = 0;
    }
  }
}
