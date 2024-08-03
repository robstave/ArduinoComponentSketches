/**
   ACS-85-0107
   ATTiny85 Metal/Noise/selection

   Sketch

   External pin 1       = Reset (not used)
   External pin 2 (PB3) = input 0 decay
   External pin 3 (PB4) = input 1 select
   External pin 4       = GND
   External pin 5 (PB0) = nc  ( or you can use as a switch)
   External pin 6 (PB1) = out pwm
   External pin 7 (PB2) = trigger
   External pin 8       = Vcc

   V 1.0  -  First Version
   The code does math on  squarewaves to create a cowbell sound ( roland style)

   In this case,

   There are 6 sections

   ClassicCB 0  - Cowbell Classic ( See ACS-85-0105)
   Xor2 1 - Cowbell Xor ( See ACS-85-0106)
   Xor3 2 - Cowbell 3Xor/other ( See ACS-85-0105)
   Xor4 3  - 4 xors (its getting noisy)
   Drop 4 - 2xor with a tweek/drop in freq
   Drop2 5 - 3xor with a tweek/drop in freq

   The "Drop" was an attempt to drop down the frequency.  It didnt turn out that
   way..perhaps because of an aliasing/whatever that the xor brings to the table.
 
   In not messing with tuning in this case.  But there is a pin if you want to toggle 
   sets of frequencies or just find a way to steal code from the others.

   Also, I did some optimizations in the code.  the "delay" is not really precise
   and the processing that happens in the loops can effect the envelope.

   Rob Stave (Rob the fiddler) ccby 2024
*/

//               ATTiny85 overview
//                     +-\/-+
//                Reset 1|    |8  VCC
// (pin3)     decay PB3 2|    |7  PB2 (pin2/int0) trigger
// (pin4)    select PB4 3|    |6  PB1 (pin1) out pwm
//                  GND 4|    |5  PB0 (pin0) nc
//

 

// If mode is classic, then there is no tuning
#define ClassicNote1 2721
#define ClassicNote2 1890
#define ClassicNote3 2200
#define ClassicNote4 2900

// dds
volatile unsigned int Acc1 = 0;
volatile unsigned int Note1 = ClassicNote1;
volatile unsigned int Acc2 = 0;
volatile unsigned int Note2 = ClassicNote2;
volatile unsigned int Acc3 = 0;
volatile unsigned int Note3 = ClassicNote3;
volatile unsigned int Acc4 = 0;
volatile unsigned int Note4 = ClassicNote4;

#define ClassicCB 0
#define Xor2 1
#define Xor3 2
#define Xor4 3
#define Drop 4
#define Drop2 5

volatile byte scene = 0;

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

  static int len;    // length of env
  static int select; // select and tune of sorts
  static uint32_t oldTime = 0;

  static bool doWacky = true;

  boolean triggered = false;

byte loop = 0;
  while (true)
  {

   if (loop % 3 == 0){
    len = analogRead(A3) >> 3; // set length
    }
    loop++;

    if (gateTriggered == true)
    {
      select = analogRead(A2); // set note
      scene = mapValue(select);
   
     
      gateTriggered = false;
      env = 255;
      triggered = true;

    
      Note1 = ClassicNote1; //
      Note2 = ClassicNote2; //
      Note3 = ClassicNote3; //
      Note4 = ClassicNote4; //
   
    }

    unsigned long currentMillis = notReallyMillis;

    if (triggered && (currentMillis - oldTime >= len))
    {
      oldTime = currentMillis;
      env--;

      if (scene == Drop || scene == Drop2)
      {
        Note1 = Note1 - (255 - env);
        Note3 = Note3 - (255 - env);
      }
    }

    if (env == 0)
    {
      triggered = false;
    }
  }
}

// I could do fancy loops and math here, but its easier these days to just ask chatgpt to redesign this if/else statement
// based on my needs.
byte mapValue(int input) {
    if (input <= 170) {
        return ClassicCB;
    } else if (input <= 341) {
        return Xor2;
    } else if (input <= 512) {
        return Xor3;
    } else if (input <= 683) {
        return Xor4;
    } else if (input <= 854) {
        return Drop;
    } else {
        return Drop2;
    }
}


ISR(TIMER0_COMPA_vect)
{

  notReallyMillis++;
  Acc1 = Acc1 + Note1;
  Acc2 = Acc2 + Note2;
  Acc3 = Acc3 + Note3;
  Acc4 = Acc4 + Note4;
  // Like DDS..but really squarewave
// dds-ish squarewaves
  bool a1 = (Acc1 & 0x8000) != 0;
  bool a2 = (Acc2 & 0x8000) != 0;
  bool a3 = (Acc3 & 0x8000) != 0;
  bool a4 = (Acc4 & 0x8000) != 0;
  // so an easy envelope is just the env variable that drops linear/
  //  byte amp = env;// env is a number from 255 to 0...its a linear drop off

  // instead, lets use a log decay.  Note...the table is has a exponential decay,
  // but ny env counter is counting down.
  // to map to the proper vaules I need to do 255 - env...no biggie
  byte amp = pgm_read_byte(&decay_tbl[255 - env]);

  if (amp == 0)
  {
    return;
  }

 

  // This is the a basic add.  If both are high, use the amplitude
  // if just one, divide by 2 ( a cheap shift)
  // else 0
  
  if (scene == ClassicCB)
  {
    if (a1 && a2)
    {
      OCR1A = amp;
    }
    else if (a1 || a2)
    {
      OCR1A = amp / 2;
    }
    else
    {
      OCR1A = 0;
    }

    return;
  }

  // Xor.  Gonna be more ringy. Note we are only using 2 of the 4 signals here
  // Drop and X02 are the same math, but the drop value also has a little
  // check in the dds that drops the frequency over time.

  if (scene == Xor2 || scene == Drop)
  {
    if (a1 ^ a2)
    {
      OCR1A = amp;
    }
    else
    {
      OCR1A = 0;
    }
    return;
  }

  // xor in another signal
  if (scene == Xor3 || scene == Drop)
  {
    if (a1 ^ a2 ^ a3)
    {
      OCR1A = amp;
    }
    else
    {
      OCR1A = 0;
    }
    return;
  }

  // xor em all

  if (scene == Xor4)
  {
    if (a1 ^ a2 ^ a3 ^ a4)
    {
      OCR1A = amp;
    }
    else
    {
      OCR1A = 0;
    }
    return;
  }


   if (a1 )
    {
      OCR1A = amp;
    } else {
      OCR1A = 0;
    }

}