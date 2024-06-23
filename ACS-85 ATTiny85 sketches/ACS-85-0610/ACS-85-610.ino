/**
 * ACS-85-0610
 * ATTiny85 4 Phase Counter/Sequencer
 *
 * External pin 1       = Reset (not used)
 * External pin 2 (PB3) = OUT1
 * External pin 3 (PB4) = OUT2
 * External pin 4       = GND
 * External pin 5 (PB0) = OUT3
 * External pin 6 (PB1) = OUT4
 * External pin 7 (PB2) = Clock
 * External pin 8       = Vcc
 *
 * V 1.0  -  First Version
 *
 * Note: This sketch has been written specifically for ATTINY85 and not Arduino uno
 *
 *
 * Counter/4 phase clock
 *
 * Takes and input clock and outputs quadrature phase
 *
 * 0: xx__xx__
 * 1: _xx__xx_
 * 2: __xx__xx
 * 3: x__xx__x
 *
 * Note, each pins is actually independent of each other, so you could do differnt patterns and even polyrhythms
 *
 * #define PATTERN_0_SIZE 8
 * boolean pattern_0[PATTERN_0_SIZE] = { HIGH, HIGH, LOW, LOW, HIGH, HIGH, LOW, LOW };
 * #define PATTERN_1_SIZE 9
 * boolean pattern_1[PATTERN_0_SIZE] = { HIGH, HIGH, HIGH, LOW, LOW, HIGH, HIGH, LOW };
 *
 * #define PATTERN_2_SIZE 11
 * boolean pattern_2[PATTERN_0_SIZE] = { LOW, LOW, HIGH, HIGH, LOW, LOW, HIGH, HIGH, LOW, HIGH, HIGH };
 *
 * #define PATTERN_3_SIZE 3
 * boolean pattern_3[PATTERN_0_SIZE] = { HIGH, LOW, LOW};
 *
 *
 * Rob Stave (Rob the fiddler) CCBY 2023
 */

//  ATTiny overview
//                        +-\/-+
//                 Reset 1|    |8  VCC
//      (pin3) OUT2  PB3 2|    |7  PB2 (pin2/int0) CLOCK
//      (pin4 OUT3   PB4 3|    |6 PB1 OUT1 (pin1)
//                   GND 4|    |5 PB0 OUT0 (pin0)
//                        ------

const int int0 = 0; // interrupt 0

volatile byte pattern;
int patternCount = 0;

#define PATTERN_0_SIZE 8
boolean pattern_0[PATTERN_0_SIZE] = {HIGH, HIGH, LOW, LOW, HIGH, HIGH, LOW, LOW};

#define PATTERN_1_SIZE 8
boolean pattern_1[PATTERN_0_SIZE] = {LOW, HIGH, HIGH, LOW, LOW, HIGH, HIGH, LOW};

#define PATTERN_2_SIZE 8
boolean pattern_2[PATTERN_0_SIZE] = {LOW, LOW, HIGH, HIGH, LOW, LOW, HIGH, HIGH};

#define PATTERN_3_SIZE 8
boolean pattern_3[PATTERN_0_SIZE] = {HIGH, LOW, LOW, HIGH, HIGH, LOW, LOW, HIGH};

volatile byte counter0 = 0;
volatile byte counter1 = 0;
volatile byte counter2 = 0;
volatile byte counter3 = 0;

void setup()
{

    PORTB = 0x00; // enable all pull-down

    DDRB = B00011011; // set PORTB to inputs except PB0

    attachInterrupt(int0, clockCounter, CHANGE);
    //  clockInt is our interrupt, clockCounter function is called when
    //  invoked on either clock edge
}
boolean setPatternValue()
{
    boolean result = LOW;

    result = pattern_0[counter0];

    if (result == HIGH)
    {
        PORTB |= _BV(PB0);
    }
    else
    {
        PORTB &= ~_BV(PB0);
    }

    counter0++;
    if (counter0 >= PATTERN_0_SIZE)
    {
        counter0 = 0;
    }

    result = pattern_1[counter1];

    if (result == HIGH)
    {
        PORTB |= _BV(PB1);
    }
    else
    {
        PORTB &= ~_BV(PB1);
    }

    counter1++;
    if (counter1 >= PATTERN_1_SIZE)
    {
        counter1 = 0;
    }

    result = pattern_2[counter2];

    if (result == HIGH)
    {
        PORTB |= _BV(PB3);
    }
    else
    {
        PORTB &= ~_BV(PB3);
    }

    counter2++;
    if (counter2 >= PATTERN_2_SIZE)
    {
        counter2 = 0;
    }

    result = pattern_3[counter3];

    if (result == HIGH)
    {
        PORTB |= _BV(PB4);
    }
    else
    {
        PORTB &= ~_BV(PB4);
    }

    counter3++;
    if (counter3 >= PATTERN_3_SIZE)
    {
        counter3 = 0;
    }
}

/**
 * Interrupt.
 * Sample the counter and determine the elapsed time.
 */
void clockCounter() // called by interrupt
{
    setPatternValue();
}

void loop()
{
    //  It is by will alone I set my loop in motion.
}
