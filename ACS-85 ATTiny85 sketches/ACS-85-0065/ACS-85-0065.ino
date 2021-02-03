/**
   ACS-85-0065
   ATTiny85 Squarewave  VCO  Dual XOR Drones
   External pin 1       = Reset (not used)
   External pin 2 (PB3) = input 0 freq
   External pin 3 (PB4) = input 1 width
   External pin 4       = GND
   External pin 5 (PB0) = output 0 output
   External pin 6 (PB1) = output 1 output
   External pin 7 (PB2) = output 2 gate
   External pin 8       = Vcc

   Two tones made up of 3 DDS accumulators.
   A dead simple DDS is really just the top bit of the int

   http://www.technoblogy.com/show?QVN

   In this case, I have the base note and a few others that are modulated a tad.
   Xor the three bools to get a note.  Its kinda Phasey.

   There are Two notes, PB0 is a bit more phasey.

   A Gate pin is there too, but you could make that an output of EVERYTHING AND'd, OR'd or XOR'd


   V 1.0  -  First Version

   TODO, GATE is not there yet

   Rob Stave (Rob the fiddler) ccby 2021
*/

//  ATTiny overview
//                      +-\/-+
//                Reset 1|    |8  VCC
// (pin3) width A3  PB3 2|    |7  PB2 (pin2) gate  TODO
// (pin4) freq  A2  PB4 3|    |6  PB1 (pin1) output b
//                  GND 4|    |5  PB0 (pin0) output a
//                       ------

volatile unsigned int note;
volatile unsigned int note2;
volatile unsigned int width;

volatile unsigned int acc1a = 0;
volatile unsigned int acc1b = 0;
volatile unsigned int acc1c = 0;

volatile unsigned int acc2a = 0;
volatile unsigned int acc2b = 0;
volatile unsigned int acc2c = 0;


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
  int VCO1HIGH = 2400;
  int pwmMin = 15; // feel free to drop this to 1
  int pwmMax = 245;  // feel free to up to 255
  while (true)
  {

    // not really needed, but I like to average my samples a bit
    // you can ditch this if want and just read direct.

    f1Sample[counter] = analogRead(A2);
    unsigned int osc1_t = (f1Sample[0] + f1Sample[1] + f1Sample[2] + f1Sample[3]) >> 2;
    note = map(osc1_t, 0, 1023, VCO1LOW, VCO1HIGH);
    note2 = note + (note >> 2);


    f2Sample[counter] = analogRead(A3);
    unsigned int osc2_t = (f2Sample[0] + f2Sample[1] + f2Sample[2] + f2Sample[3]) >> 2;
    width = map(osc2_t, 0, 1023, pwmMin, pwmMax);

    counter++;
    if (counter > 3) {
      counter = 0;
    }
  }
}

ISR(TIMER0_COMPA_vect)
{

  // First note.  I do a little of the math in the loop.
  // if your hurting for clock cycles here, things like
  //  note + (width >> 3); can be done in the loop as well.
  //
  // just replace
  // acc1b = acc1b + note + (width >> 3);
  // with
  // acc1b = acc1b + noteFM1
  // and  put
  // noteFM1 = note + (width >> 3); in the loop

  acc1a = acc1a + note;
  bool bit1a = ((acc1a >> 8) > 0x7F);

  // vibrato up
  acc1b = acc1b + note + (width >> 3);
  bool bit1b = ((acc1b >> 8) > 0x7F);

  // vibrato down
  acc1c = acc1c + note - (width >> 3);
  bool bit1c = ((acc1c >> 8) > 0x7F);

  // and xor the mess
  if ((bit1a ^ bit1b ^ bit1c) == true)  {
    bitSet(PORTB, 0);
  } else {
    bitClear(PORTB, 0);
  }

  // Second note
  acc2a = acc2a + note2;
  bool bit2a = ((acc2a >> 8) > 0x7F);

  acc2b = acc2b + note2 + (width >> 4);
  bool bit2b = ((acc2b >> 8) > 0x7F);

  acc2c = acc2c + note2 - (width >> 4);
  bool bit2c = ((acc2c >> 8) > 0x7F);


  if ((bit2a ^ bit2b ^ bit2c) == true ) {
    bitSet(PORTB, 1);

  } else {
    bitClear(PORTB, 1);
  }

  // you could ditch the gate and output other combos here to pin 2


}
