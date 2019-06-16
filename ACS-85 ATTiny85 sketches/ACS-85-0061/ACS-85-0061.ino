/**
   ACS-85-0061 ATTiny85 - VCO - DDS Squarewave with LFO PWM.

   External pin 2 = LFO width
   External pin 3 = LFO speed
   External pin 5 = vco freq  Squarewave
   External pin 6 = LFO modulated pulse width Squarewave
   External pin 7 = Freq of vco

    ATTiny overview
                            +-\/-+
                     Reset 1|    |8  VCC
         (pin3) width  PB3 2|    |7  PB2 freq
         (pin4) speed  PB4 3|    |6  PB1 (pin1)  f1
                       GND 4|    |5  PB0 (pin0)  f0
                            ------

   Description:

   DDS based (http://www.technoblogy.com/show?QVN) although it does not really reference a wavetable.
   Instead, the highest bit is used to produce a squarewave.

   A second squarewave is created as well using an LFO to detune the first pitch.

   The two pins should be summed via resistors (or op amp)

   Consider a Mix or at least the means to add in F1 incrementally.


   The prescaler is set to 64  (CS00=1 and CS01=1)
   see http://www.atmel.com/Images/atmel-2586-avr-8-bit-microcontroller-attiny25-attiny45-attiny85_datasheet.pdf
   or google ATTINY85 data sheet and look for that table.

   cs02 cs01 cs00
    0    0    1   Clk/1
    0    1    0   Clk/8
    0    1    1   Clk/64

   So if we set OCR0A = 5

   8Mhz/(64*(OCR0A +1)) = 8mhz/(64*6) = 20833
   So we are interupting at 20.8Khz

   V 1.0  -  First Version

   Rob Stave (Rob the Fiddler) CCBY 2019

*/

volatile unsigned int Acc1; // main freq

volatile unsigned int Note1 = 857; // tuning word
volatile unsigned int Delta = 0;

// the setup function runs once when you press reset or power the board
void setup()
{

  DDRB = B00000011; //Set port B output bits

  // initialize timer
  noInterrupts(); // disable all interrupts

  TCCR0A = 0;
  TCCR0B = 0;

  TCCR0A |= (1 << WGM01); //Start timer in CTC mode Table 11.5

  OCR0A = 5; //CTC Compare value...this is fairly arbitrary and you can change, but have to adjust math.

  TCCR0B |= (1 << CS00) | (1 << CS01); // Prescaler =64 Table 11.6

  TIMSK |= (1 << OCIE0A); //Enable CTC interrupt see 13.3.6

  // start the timer, prescaler
  TCCR1 = (1 << CTC1) | (7 << CS10); // CTC  mode, div64
  OCR1C = 0.001 * F_CPU / 64 - 1;    // 1ms, F_CPU @16MHz, div64
  TIMSK |= (1 << OCIE1A);

  interrupts(); // enable all interrupts
}

volatile unsigned long milliseconds;

// Set second timer to implement "millis"
ISR(TIMER1_COMPA_vect)
{
  milliseconds++;
}

unsigned long millisTimer1()
{
  return milliseconds;
}

ISR(TIMER0_COMPA_vect) // timer compare interrupt service routine
{
  Acc1 = Acc1 + Note1;

  uint8_t result1 = (Acc1 >> 8) & 0x80;

  if (result1 > 0)
  {
    bitSet(PORTB, 0);
  }
  else
  {
    bitClear(PORTB, 0);
  }

  uint8_t result2 = (Acc1 >> 8);
  uint8_t mark = 128 - Delta;




  if (result2  > mark)
  {
    bitSet(PORTB, 1);
  }
  else
  {
    bitClear(PORTB, 1);
  }
}

void loop()
{

  unsigned long previousMillis = 0; // will store last time LED was updated
  int loopSpeed = 40;
  signed int lfoCounter = 0;
  boolean lfoState = true;
  byte lfoWidth = 8;


  // no need to check speed and LFO all that quickly
  // so we space that out a bit
  byte sensorCounter = 0;
  byte sensorLimit = 100;
  byte checkState = 0;


  while (true)
  {
    // get frequency
    int sample = analogRead(A1);
    Note1 = map(sample, 0, 1023, 2200, 300);

    if (sensorCounter > sensorLimit)
    {
      if (checkState % 2 == 0)
      {
        int sample2 = analogRead(A3);
        lfoWidth = map(sample2, 0, 1023, 1, 80);
      }
      else
      {
        int sample3 = analogRead(A2);
        loopSpeed = map(sample3, 0, 1023, 3, 80);
      }

      sensorCounter = 0;
      checkState++;
    }
    sensorCounter++;

    unsigned long currentMillis = millisTimer1();
    if (currentMillis - previousMillis >= loopSpeed)
    {

      //Manage LFO and Note2 tuning word
      if (lfoState == true)
      {
        lfoCounter++;
        if (lfoCounter > lfoWidth)
        {
          lfoState = false;
        }
      }
      else
      {
        lfoCounter--;
        if (lfoCounter < -1 * lfoWidth)
        {
          lfoState = true;
        }
      }

      Delta =  lfoCounter;
      previousMillis = currentMillis;
    }
  }
}
