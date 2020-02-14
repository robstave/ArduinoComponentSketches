/**
   ACS-85-0040
   ATTiny85 Squarewave  VCO  with adjustable duty cycle and chip tune

   External pin 1       = Reset (not used)
   External pin 2 (PB3) = input 0 freq
   External pin 3 (PB4) = input 1 width
   External pin 4       = GND
   External pin 5 (PB0) = output 0 output ( or chip tune sound )
   External pin 6 (PB1) = output 1 output with duty cycle
   External pin 7 (PB2) = output 2 switch between regular or chip tone
   External pin 8       = Vcc

   V 1.0  -  First Version

   Chiptone sound really is set up for 8Mhz ATTINY.  If you run this on 16mhz or more, you may need
   to mess with the counter since there really not a delay, it just kinda works with the timing of the loop.

   Rob Stave (Rob the fiddler) ccby 2020
*/

//  ATTiny overview
//                      +-\/-+
//                Reset 1|    |8  VCC
// (pin3) width A3  PB3 2|    |7  PB2 (pin2) switch
// (pin4) freq  A2  PB4 3|    |6  PB1 (pin1) output w duty
//                  GND 4|    |5  PB0 (pin0) output
//                       ------

volatile unsigned int Acc;
volatile unsigned int Note = 857;  // Middle C
volatile unsigned int Width = 127; // 50%
volatile unsigned int mode = LOW;
volatile byte effects = 0;

void setup()
{

  DDRB = B00000011; //set output bits

  noInterrupts(); // disable all interrupts
  // Set up Timer/Counter0 for 20kHz interrupt to output samples.
  TCCR0A = 3 << WGM00;             // Fast PWM
  TCCR0B = 1 << WGM02 | 2 << CS00; // 1/8 prescale
  TIMSK = 1 << OCIE0A;             // Enable compare match, disable overflow
  OCR0A = 49;                      // Divide by 400

  interrupts(); // enable all interrupts
}

void loop()
{

  // Used for sample averaging
  int f1Sample[4] = {0, 0, 0, 0};
  int f2Sample[4] = {0, 0, 0, 0};
  byte counter = 0;

  int VCO1LOW = 200;
  int VCO1HIGH = 2200;
  int pwmMin = 10;
  int pwmMax = 253;
  while (true)
  {

    f1Sample[counter] = analogRead(A2);
    unsigned int osc1_t = (f1Sample[0] + f1Sample[1] + f1Sample[2] + f1Sample[3]) >> 2;
    Note = map(osc1_t, 0, 1023, VCO1LOW, VCO1HIGH);

    f2Sample[counter] = analogRead(A3);
    unsigned int osc2_t = (f2Sample[0] + f2Sample[1] + f2Sample[2] + f2Sample[3]) >> 2;
    Width = map(osc2_t, 0, 1023, pwmMin, pwmMax);

    mode = digitalRead(2); // read the input pin


    counter++;
    if (counter >= 4) {
      counter = 0;
    }

    effects++;
  }
}

ISR(TIMER0_COMPA_vect)
{

  int localNote = Note;

  // Chip tone sound
  if (mode == HIGH)
  {
    if (effects > 127)
    {
      localNote = localNote + Note;
      if (effects > 168)
      {
        localNote = localNote + Note;
        if (effects > 200)
        {
          localNote = localNote + Note;
        }
      }
    }
  }

  Acc = Acc + localNote;

  unsigned int upper8bits = (Acc >> 8);

  if (upper8bits > 0x7F)
  {
    bitSet(PORTB, 0);
  } else {
    bitClear(PORTB, 0);
  }

  if (upper8bits > Width) {
    bitSet(PORTB, 1);
  } else {
    bitClear(PORTB, 1);
  }
}
