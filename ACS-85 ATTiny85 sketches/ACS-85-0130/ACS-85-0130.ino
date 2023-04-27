/*

   ACS-85-0130
   VCO/Drone  Sub super square

   Attempt at a super saw...but really square waves with two detunes and a sub.
   The Detune is based on the equation power(2,n/12)  so if there are 100 cents in a semitone,
   50 cents would be 440 * power(2, 0.5/12) and 440 * power(2,-0.5/12)

   Can you tell the difference...well..maybe thats another experiment.


   External pin 1       = Reset (not used)
   External pin 2 (PB3) = Input freq
   External pin 3 (PB4) = Input Detune Spread
   External pin 4       = GND
   External pin 5 (PB0) =
   External pin 6 (PB1) = Output (PWM)
   External pin 7 (PB2) = Input Detune Intensity
   External pin 8       = Vcc

   Consider a few things like a hard sync? you can periodically sync it all if a bool is set.
   Or an LFO...but there is no easy way to control the rate with 3 analog inputs.

   A fun math shortcut is rather than attenuate the detune frequency values, just add +/- "Input Detune Intensity"


   V 1.0

   todo  make this 1v/oct

  Rob Stave (Rob the fiddler) CCBY 2022

*/


// An attempt at an even tempered spread.  For example
// Power(2, n/12)  where n is a number from 0 to 1 in .05 increments
// Doubles and floats are the same and the math was just not working well.
/// So I multiplied by 1000
// you could save a bit more by doing bitshifting ( 1024 * x >> 10)

const int detuneUp[21] PROGMEM = {
  1000, // 0
  1003, // 0.05
  1006, //.1
  1009,
  1012,
  1015,
  1018,
  1020,
  1023,
  1026,
  1029,
  1032,
  1035,
  1038,
  1041, // .7
  1044,
  1047,
  1050,
  1053,
  1056,
  1059 // 1
};

const int detuneDown[21] PROGMEM = {
  1000,
  997,
  994,
  991,
  989,
  986,
  983,
  980,
  977,
  974,
  972,
  969,
  966,
  963,
  960,
  958,
  955,
  952,
  949,
  947,
  944
};



#define VCO1_HIGH 4000
#define VCO1_LOW 400

volatile unsigned int Acc;
volatile unsigned int AccSub;
volatile unsigned int AccDetUp;
volatile unsigned int AccDetDown;

volatile unsigned int Note = 857;
volatile unsigned int SubNote = 0;
volatile unsigned int DetUpNote = 0;
volatile unsigned int DetDownNote = 0;

volatile  int doTest = 0;

volatile byte  DetuneAmplitude = 100;  // a number from 0 to 128
volatile byte  SubAmplitude = 100;

void setup()
{
  SetupDDS();
}

void SetupDDS()
{
  // Enable 64 MHz PLL and use as source for Timer1
  // PLLCSR = 1<<PCKE | 1<<PLLE;
  // Below is supposed to be better....not sure why
  PLLCSR |= (1 << PLLE); // PLL enable for asynchronous mode

  while ((PLLCSR & (1 << PLOCK)) == 0x00)
  {
    // Do nothing until plock bit is set
  }
  PLLCSR |= (1 << PCKE); // Sets the timer/clock1 source to RC oscillator

  // Set up Timer/Counter1 for PWM output
  TIMSK = 0;                                    // Timer interrupts OFF
  TCCR1 = 1 << PWM1A | 2 << COM1A0 | 1 << CS10; // PWM A, clear on match, 1:1 prescale
  pinMode(1, OUTPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(2, INPUT);

  pinMode(0, OUTPUT);

  // Set up Timer/Counter0 for 20kHz interrupt to output samples.
  TCCR0A = 3 << WGM00;             // Fast PWM
  TCCR0B = 1 << WGM02 | 2 << CS00; // 1/8 prescale
  TIMSK = 1 << OCIE0A;             // Enable compare match, disable overflow
  OCR0A = 60;                      // Divide by 61
}



void loop()
{

  byte count = 0;
  int spread = 0;
  int detuneTop = 1000;
  int detuneBottom = 1000;

  while (true) {
    //Read freq
    int osc1_t = analogRead(A3);
    Note = map(osc1_t, 0, 1023, VCO1_LOW, VCO1_HIGH);

    // Calculate sub octave
    SubNote = Note >> 1;

    // check 50% of the time
    if ( count % 2 == 0 ) {
      spread = map(analogRead(A2), 0, 1023, 0, 20);
      detuneTop = pgm_read_word_near(detuneUp +  spread);
      detuneBottom = pgm_read_word_near(detuneDown +  spread);
    }

    // can save some loop cycles if you move this into the detune if statement
    unsigned int maxNote =   ((long)Note * (long)detuneTop) / 1000;
    unsigned int minNote =  ((long)Note * (long)detuneBottom) / 1000;

    // I left room for an LFO on this to tighten the spread...but really there is
    // only 3 analog reads.  Maybe for a nano
    //DetUpNote = map(lfoModifier, 0, 255, Note, maxNote);
    //DetDownNote = map(255 - lfoModifier, 0, 255, minNote, Note);

    // Set notes for DDS
    DetUpNote =   maxNote;
    DetDownNote =  minNote;

    // check 20% of the time
    if ( count % 5 == 0 ) {
      // This could go as high as 127 rather than 100
      DetuneAmplitude = map(analogRead(A1), 0, 1023, 0, 100);
    }



    count++;
  }
}

ISR(TIMER0_COMPA_vect)
{

  // DDS stuff
  Acc = Acc + Note;
  AccSub = AccSub + SubNote;
  AccDetUp = AccDetUp + DetUpNote;
  AccDetDown = AccDetDown + DetDownNote;

  // Sum em up
  int16_t total = 0;

  //  Squarewave from the Accumulator.
  if  ((Acc >> 8) > 127) {
    total = total + 127;
  } else {
    total = total - 127;
  }

  // Octave lower
  if ((AccSub >> 8) > 127 )   {
    total = total + DetuneAmplitude;
  } else {
    total = total - DetuneAmplitude;
  }

  // Detune up
  if  ((AccDetUp >> 8) > 127 )   {
    total = total + DetuneAmplitude;
  } else {
    total = total - DetuneAmplitude;
  }


  // Detune lower
  if ( (AccDetDown >> 8) > 127 ) {
    total = total + DetuneAmplitude;
  } else {
    total = total - DetuneAmplitude;
  }

  // divide by 4 and shift back to 0->255
  total = total >> 2;
  total = total + 128;

  //  Just in case
  if ( total > 255) {
    total = 255;
  } else if (total < 0 ) {
    total = 0;
  }

  OCR1A = total;

}
