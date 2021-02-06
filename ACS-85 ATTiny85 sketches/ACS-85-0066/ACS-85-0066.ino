/**
   ACS-85-0066
   ATTiny85 Squarewave  VCO with detuned oct and fifth
   External pin 1       = Reset (not used)
   External pin 2 (PB3) = input 0 width
   External pin 3 (PB4) = input 1 freq
   External pin 4       = GND
   External pin 5 (PB0) = output 0 output
   External pin 6 (PB1) = output 1 output oct below detuned
   External pin 7 (PB2) = output 2 oct below plus fifth det
   External pin 8       = Vcc
   V 1.0  -  First Version

   Drone VCO.  Three tones, you need to sum together ( as they are squarewave)

   PB0 is the root
   PB1 is octable below and detunes by width/2
   PB2 is octave above plus a fifth  ( 3:2) and detunes up

   Rob Stave (Rob the fiddler) ccby 2021
*/

//  ATTiny overview
//                      +-\/-+
//                Reset 1|    |8  VCC
// (pin3) width A3  PB3 2|    |7  PB2 (pin2) output c root - octave
// (pin4) freq  A2  PB4 3|    |6  PB1 (pin1) output b root - octave + fifth
//                  GND 4|    |5  PB0 (pin0) output a root
//                       ------


#define VCO1LOW  200
#define VCO1HIGH  2400
#define limitMin 0
#define limitMax 30

volatile unsigned int note1;
volatile unsigned int note2;
volatile unsigned int note3;

volatile unsigned int width;

volatile unsigned int acc1 = 0;
volatile unsigned int acc2 = 0;
volatile unsigned int acc3 = 0;

void setup()
{

  DDRB = B00000111; //set output bits

  noInterrupts(); // disable all interrupts
  // Set up Timer/Counter0 for 20kHz interrupt to output samples.
  TCCR0A = 3 << WGM00;             // Fast PWM
  TCCR0B = 1 << WGM02 | 2 << CS00; // 1/8 prescale
  TIMSK = 1 << OCIE0A;             // Enable compare match, disable overflow
  OCR0A = 49;                      // Divide by 400

  interrupts(); // enable all interrupts
}


ISR(TIMER0_COMPA_vect)
{

  acc1 = acc1 + note1;
  bool bit1 = ((acc1 >> 8) > 0x7F);

  acc2 = acc2 + note2 ;
  bool bit2 = ((acc2 >> 8) > 0x7F);

  acc3 = acc3 + note3;
  bool bit3 = ((acc3 >> 8) > 0x7F);

  if (bit1 == true)  {
    bitSet(PORTB, 0);
  } else {
    bitClear(PORTB, 0);
  }

  if (bit2 == true)  {
    bitSet(PORTB, 1);
  } else {
    bitClear(PORTB, 1);
  }

  if (bit3 == true)  {
    bitSet(PORTB, 2);
  } else {
    bitClear(PORTB, 2);
  }

}

void loop()
{

  // Used for sample averaging
  int f1Sample[4] = {0, 0, 0, 0};
  int f2Sample[4] = {0, 0, 0, 0};
  byte counter = 0;

  int note;

  int mod2 = 0;
  int mod3 = 0;

  while (true)
  {

    // not really needed, but I like to average my samples a bit
    f1Sample[counter] = analogRead(A2);
    unsigned int osc1_t = (f1Sample[0] + f1Sample[1] + f1Sample[2] + f1Sample[3]) >> 2;
    note = map(osc1_t, 0, 1023, VCO1LOW, VCO1HIGH);

    f2Sample[counter] = analogRead(A3);
    unsigned int osc2_t = (f2Sample[0] + f2Sample[1] + f2Sample[2] + f2Sample[3]) >> 2;
    width = map(osc2_t, 0, 1023, limitMin, limitMax);

    counter++;
    if (counter > 3) {
      counter = 0;
    }

    mod2 = width;
    mod3 = mod2 >> 1;

    note1 = note;
    note = (note >> 1);
    //oct below
    note2 = note + mod2;
    // fifth above oct below
    note3 = (( note + note + note) >> 1) - mod3;

  }
}
