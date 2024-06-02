/**
   ACS-85-0375
   ATTiny85 euclidan   simple counter

   I grabbed the nice 16 step matrix from HAGIWO.
   https://www.youtube.com/watch?v=lkoBfiq6KPY

   AI extrapolated the others

   It just pretty much clocks through the indices.
   In this case Im clocking though size 16 and 19, but
   pick any one you want



   External pin 1       = Reset (not used)
   External pin 2 (PB3) = input Counts (A3)
   External pin 3 (PB4) = input reset
   External pin 4       = GND
   External pin 5 (PB0) = output A
   External pin 6 (PB1) = output B
   External pin 7 (PB2) = clock
   External pin 8       = Vcc
   V 1.0  -  First Version

   Rob Stave (Rob the fiddler) ccby 2024
*/

//  ATTiny overview
//                           +-\/-+
//                    Reset 1|    |8  VCC
//     (pin3) count     PB3 2|    |7  PB2 (pin2) clock
//     (pin4) reset 3   PB4 3|    |6  PB1 (pin1) output B
//                      GND 4|    |5  PB0 (pin0) output A
//                           ------

#define OUT1 PB1
#define OUT2 PB0
#define CLOCK_PIN PB2
#define RESET_PIN PB4
#define DIVBY_PIN PB3

volatile unsigned int counter = 0;
volatile bool doReset = false;

const int clockInt = 0;  // digital pin 2 is now interrupt 0

const static byte euc16[17][16] PROGMEM = {  //euclidian rythm
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0 },
  { 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0 },
  { 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0 },
  { 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0 },
  { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 },
  { 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0 },
  { 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1 },
  { 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1 },
  { 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1 },
  { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1 },
  { 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};

const static byte euc8[9][8] PROGMEM = {  // Euclidean rhythm
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 1, 0, 0, 0 },
  { 1, 0, 1, 0, 0, 1, 0, 0 },
  { 1, 0, 1, 0, 1, 0, 1, 0 },
  { 1, 0, 1, 1, 0, 1, 1, 0 },
  { 1, 1, 0, 1, 1, 0, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 0 },
  { 1, 1, 1, 1, 1, 1, 1, 1 }
};


const static byte euc9[10][9] PROGMEM = {  // Euclidean rhythm
  { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 1, 0, 0, 0, 0 },
  { 1, 0, 0, 1, 0, 0, 1, 0, 0 },
  { 1, 0, 1, 0, 1, 0, 1, 0, 0 },
  { 1, 0, 1, 0, 1, 1, 0, 1, 0 },
  { 1, 0, 1, 1, 0, 1, 1, 0, 1 },
  { 1, 1, 0, 1, 1, 1, 0, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 0 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};


const static byte euc19[20][19] PROGMEM = {  // Euclidean rhythm
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0 },
  { 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0 },
  { 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0 },
  { 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0 },
  { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0 },
  { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0 },
  { 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1 },
  { 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0 },
  { 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1 },
  { 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0 },
  { 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1 },
  { 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0 },
  { 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1 },
  { 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};


void setup() {
  // Set output pins
  DDRB = B00000011;  //set output bits

  // Attach interrupts
  attachInterrupt(clockInt, clockCounter, RISING);
}

void clockCounter()  // called by interrupt
{
  if (doReset) {
    counter = 0;
    doReset = false;
  } else {
    counter++;
  }
}

bool euclideanRhythm8(int index, int counts) {
  index = index % 8;
  return (pgm_read_byte(&(euc8[counts][index]))) == 1;
}

bool euclideanRhythm9(int index, int counts) {
  index = index % 9;
  return (pgm_read_byte(&(euc9[counts][index]))) == 1;
}

bool euclideanRhythm16(int index, int counts) {
  index = index % 16;
  return (pgm_read_byte(&(euc16[counts][index]))) == 1;
}

bool euclideanRhythm19(int index, int counts) {
  index = index % 19;
  return (pgm_read_byte(&(euc19[counts][index]))) == 1;
}

void loop() {

  bool resetActive = false;
  bool resetHigh = false;
  int counts1 = 0;
  int counts2 = 0;
  int value = 0;

  while (true) {

    value = analogRead(A3);
    counts1 = map(value, 0, 1023, 0, 16);
    counts2 = map(value, 0, 1023, 0, 19);

    bool resetHigh = digitalRead(RESET_PIN);

    // latch the reset
    if (resetHigh && !resetActive) {
      doReset = true;
      resetActive = true;
    }

    if (!resetHigh && resetActive) {
      resetActive = false;
    }
  }


  if (digitalRead(CLOCK_PIN)) {

    bool outputPB0 = euclideanRhythm16(counter, counts1);
    digitalWrite(PB0, outputPB0);
    bool outputPB1 = euclideanRhythm19(counter, counts2);
    digitalWrite(PB1, outputPB1);
  } else {
    digitalWrite(PB0, false);
    digitalWrite(PB1, false);
  }


  delay(2);  // Short delay to stabilize readings
}
