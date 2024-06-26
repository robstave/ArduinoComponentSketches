

/**
 * ACS-85-0320
 * ATTiny85 Divide by 2,3,4,5
 *
 * Divide the clock signal by 2,3,4,5 across the 4 output pins.
 *
 *
 * We will put an interrupt procedure on that to count clock
 * cycles and output digital writes to do divide by 2,3,4,5.
 * The  approach here can obviously be improved if you need speed.
 * For example, rather than writing to each pin, figure out your
 * values and write to say PortB all at once.

 *
 * External pin 1       = Reset (not used)
 * External pin 2 (PB3) = Divide By
 * External pin 3 (PB4) = Divide N+2
 * External pin 4       = GND
 * External pin 5 (PB0) = Divide N+1
 * External pin 6 (PB1) = Divide N
 * External pin 7 (PB2) = Clock
 * External pin 8       = Vcc

 *
 * V 1.0  -  First Version
 *
 * Note: This sketch has been written specifically for ATTINY85 and not Arduino uno
 *
 * There are actually different types of pin interrupts.
 * INT0 has a higher priority that PCINT
 * see here for a good overview of that
 * http://thewanderingengineer.com/2014/08/11/pin-change-interrupts-on-attiny85/
 *
 * Rob Stave (Rob the fiddler) CCBY 2015
 */

//  ATTiny overview
//                        +-\/-+
//                 Reset 1|    |8  VCC
//      (pin3) DivBy PB3 2|    |7  PB2 (pin2/int0) CLOCK
//      (pin4) DIVC  PB4 3|    |6  PB1 (pin1) DIVA
//                   GND 4|    |5  PB0 (pin0) DIVB
//                        ------

const int int0 = 0; // interrupt 0

 

// Counters for divide by n
int qACount = 0;
int qBCount = 0;
int qCCount = 0;
int qDCount = 0;

int divideByN = 2;

int prevSample1 = 0; // Sample timestamp
#define VAR1 A3      // Analog read

// A hysteresis of sorts is used to prevent values from switching back and forth.
int sampleValue = 0;
#define HYSTERESIS 3

void setup()
{
  attachInterrupt(int0, clockCounter, CHANGE);
  //  clockInt is our interrupt, clockCounter function is called when
  //  invoked on either clock edge

  DDRB = B00010011; // set PORTB to outputs except PB2 for the clock
}

void clockCounter() // called by interrupt
{
  divByA();
  divByB();
  divByC();
}

void divByA()
{
  qACount++;
  if (qACount >= (divideByN))
  {
    qACount = 0;
    PORTB ^= (_BV(PB0));
  }
}

void divByB()
{
  qBCount++;
  if (qBCount >= (divideByN + 1))
  {
    qBCount = 0;
    PORTB ^= (_BV(PB1));
  }
}

void divByC()
{
  qCCount++;
  if (qCCount >= (divideByN + 2))
  {
    qCCount = 0;
    PORTB ^= (_BV(PB4));
  }
}

void loop()
{
  int millsSample = millis();

  // Sample every 40ms
  if (millsSample - prevSample1 > 50)
  {
    prevSample1 = millsSample;

    int temp = analogRead(VAR1);
    if (abs(sampleValue - temp) > HYSTERESIS)
    {
      sampleValue = temp;
    }
    divideByN = map(sampleValue, 0, 1023, 2, 12); // map the value to divisors
  }
}
