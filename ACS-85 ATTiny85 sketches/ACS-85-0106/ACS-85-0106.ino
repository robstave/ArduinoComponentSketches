/**
   ACS-85-0106
   ATTiny85 Cowbell XOR Edition

   Cowbell simulator.  Same as 105, but its a little more FM sounding
   as there is an XOR involved.

   Sketch

   External pin 1       = Reset (not used)
   External pin 2 (PB3) = input 0 decay
   External pin 3 (PB4) = input 1 freq/tune
   External pin 4       = GND
   External pin 5 (PB0) = nc  ( or you can use as a switch)
   External pin 6 (PB1) = out pwm
   External pin 7 (PB2) = trigger
   External pin 8       = Vcc

   V 1.0  -  First Version
   Any further extensions to this will be treated as a sequencer.
   The code as is sums two squarewaves to create a cowbell sound ( roland style)

   You can,  if you would like, disable the tuning and just stick with the classic
   frequencies or add a switch to toggle that.  The code as is allows for tuning.
   Its not going to be super.  If you want to replace the lpw with a bandpass you
   can achieve a closer sound.  Your going to have to google that though

   Rob Stave (Rob the fiddler) ccby 2023
*/

//               ATTiny85 overview
//                     +-\/-+
//                Reset 1|    |8  VCC
// (pin3)     decay PB3 2|    |7  PB2 (pin2/int0) trigger
// (pin4)      tune PB4 3|    |6  PB1 (pin1) out pwm
//                  GND 4|    |5  PB0 (pin0) classic = high
//

// twin notes if tuning is involved.  Not a lot of research was
// done to get these numbers...they kinda just work
#define Note1Min 1000
#define Note1Max 3000
#define Note2Min 600
#define Note2Max 2400

// If mode is classic, then there is no tuning
#define ClassicNote1 2721
#define ClassicNote2 1890

// dds
volatile unsigned int Acc1 = 0;
volatile unsigned int Note1 = ClassicNote1; // cow freq 1
volatile unsigned int Acc2 = 0;
volatile unsigned int Note2 = ClassicNote2; // Middle C

// dunno if my interrupts are messing with millis()..so this this
// counter does basically the same thing...more like 1/10 of millis
volatile unsigned long notReallyMillis = 0;

const int clockInt = 0; // digital pin 2 is now interrupt 0

const uint8_t decay_tbl[256] PROGMEM = {
    255, 253, 250, 248, 245, 243, 240, 238,
    235, 233, 230, 228, 226, 223, 221, 219,
    216, 214, 212, 210, 208, 205, 203, 201,
    199, 197, 195, 193, 191, 189, 187, 185,
    183, 181, 179, 178, 176, 174, 172, 170,
    168, 167, 165, 163, 161, 160, 158, 156,
    155, 153, 152, 150, 148, 147, 145, 144,
    142, 141, 139, 138, 136, 135, 133, 132,
    130, 129, 128, 126, 125, 123, 122, 121,
    119, 118, 117, 116, 114, 113, 112, 111,
    109, 108, 107, 106, 104, 103, 102, 101,
    100, 99, 98, 96, 95, 94, 93, 92,
    91, 90, 89, 88, 87, 86, 85, 84,
    83, 82, 81, 80, 79, 78, 77, 76,
    75, 74, 73, 72, 71, 71, 70, 69,
    68, 67, 66, 65, 64, 64, 63, 62,
    61, 60, 60, 59, 58, 57, 57, 56,
    55, 54, 54, 53, 52, 51, 51, 50,
    49, 49, 48, 47, 46, 46, 45, 45,
    44, 43, 43, 42, 41, 41, 40, 40,
    39, 38, 38, 37, 37, 36, 35, 35,
    34, 34, 33, 33, 32, 32, 31, 31,
    30, 30, 29, 29, 28, 28, 27, 27,
    26, 26, 26, 25, 25, 24, 24, 23,
    23, 23, 22, 22, 21, 21, 21, 20,
    20, 20, 19, 19, 19, 18, 18, 18,
    17, 17, 17, 16, 16, 16, 15, 15,
    15, 15, 14, 14, 14, 13, 13, 13,
    13, 13, 12, 12, 12, 12, 11, 11,
    11, 11, 11, 10, 10, 10, 10, 10,
    10, 9, 9, 9, 9, 9, 9, 9,
    9, 8, 4, 4, 2, 1, 0, 0};

void setup()
{

  noInterrupts(); // disable all interrupts
  // Enable 64 MHz PLL and use as source for Timer1
  PLLCSR = 1 << PCKE | 1 << PLLE;

  // Set up Timer/Counter1 for PWM output
  TIMSK = 0; // Timer interrupts OFF

  // PWM Generation -timer 1 pb1
  TCCR1 = (1 << PWM1A) |               // PWM, output on PB1, compare with OCR1A (see interrupt below), reset on match with OCR1C
          (1 << COM1A1) | (1 << CS10); // no prescale
  OCR1C = 0xff;                        // 255

  DDRB = B00000010; // set output bits
  // Set up Timer/Counter0 for 20kHz interrupt to output samples.
  TCCR0A = 3 << WGM00;             // Fast PWM
  TCCR0B = 1 << WGM02 | 2 << CS00; // 1/8 prescale
  TIMSK = 1 << OCIE0A;             // Enable compare match, disable overflow
  OCR0A = 49;                      // Divide by 400

  attachInterrupt(clockInt, clockCounter, RISING);

  // pinMode(0, INPUT_PULLUP);

  interrupts(); // enable all interrupts
}

volatile bool gateTriggered = false;
volatile uint8_t env = 0; // envelope state (0-255)

void clockCounter() // called by interrupt
{
  env = 255;
  gateTriggered = true;
}

void loop()
{

  static int len;   // length of env
  static int notes; // tune of sorts
  static uint32_t oldTime = 0;

  boolean triggered = false;

  while (true)
  {

    len = analogRead(A3) >> 3; // set length

    if (gateTriggered == true)
    {
      notes = analogRead(A2); // set note
    }

    // if (digitalRead(0) == LOW) {
    if (notes < 25)
    {

      Note1 = ClassicNote1; // cow freq 1
      Note2 = ClassicNote2; // Middle C
    }
    else
    {
      Note1 = map(notes, 0, 1023, Note1Min, Note1Max);
      Note2 = map(notes, 0, 1023, Note2Min, Note2Max);
    }

    if (gateTriggered == true)
    {
      gateTriggered = false;
      env = 255;
      triggered = true;
    }

    unsigned long currentMillis = notReallyMillis;

    if (triggered && (currentMillis - oldTime >= len))
    {
      oldTime = currentMillis;
      env--;
    }

    if (env == 0)
    {
      triggered = false;
    }
  }
}

ISR(TIMER0_COMPA_vect)
{

  notReallyMillis++;
  Acc1 = Acc1 + Note1;
  Acc2 = Acc2 + Note2;
  byte x1 = (Acc1 >> 8) & 0x80;
  byte x2 = (Acc2 >> 8) & 0x80;

  // so an easy envelope is just the env variable that drops linear/
  //  byte amp = env;// env is a number from 255 to 0...its a linear drop off

  // instead, lets use a log decay.  Note...the table is has a exponential decay,
  // but ny env counter is counting down.
  // to map to the proper vaules I need to do 255 - env...no biggie
  byte amp = pgm_read_byte(&decay_tbl[255 - env]);

  // sum the waveforms ( consider an xor for something else and wacky)

  bool a1 = x1 > 100;
  bool a2 = x2 > 100;

  if (a1 ^ a2)
  {
    OCR1A = amp;
  }
  else
  {
    OCR1A = 0;
  }
}
