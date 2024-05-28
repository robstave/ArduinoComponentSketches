/**
   ACS-85-0641
   ATTiny85 afrocuban rhythm

  see https://www.thejazzpianosite.com/jazz-piano-lessons/jazz-genres/afro-cuban-latin-jazz/



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

const static byte rhy[10][16] PROGMEM = {              // rythm
  { 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0 },  // son clava 2/3
  { 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0 },  // son clava 3/2
  { 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0 },
  { 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0 },
  { 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1 },  // cascara
  { 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1 },  //cencerro
  { 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1 },  // conga
  { 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1 },  // cowbell
  { 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0 },  // tumbao
  { 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0 }
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



bool calcRhy(int index, int counts) {
  index = index % 8;
  return (pgm_read_byte(&(rhy[counts][index]))) == 1;
}

void loop() {

  bool resetActive = false;
  bool resetHigh = false;
  int counts1 = 0;
  int counts2 = 0;
  int value = 0;

  while (true) {
    value = map(analogRead(A3), 0, 1023, 0, 16);



    switch (value) {
      case 1:
        counts1 = 2;
        counts2 = 0;
        break;
      case 2:
        counts1 = 2;
        counts2 = 1;
        break;
      case 3:
        counts1 = 0;
        counts2 = 2;
        break;
      case 4:
        counts1 = 2;
        counts2 = 1;
        break;
      case 5:
        counts1 = 3;
        counts2 = 2;
        break;
      case 6:
        counts1 = 4;
        counts2 = 2;
        break;

      case 7:
        counts1 = 4;
        counts2 = 0;
        break;
      case 8:
        counts1 = 4;
        counts2 = 1;
        break;

      case 9:
        counts1 = 5;
        counts2 = 0;
        break;
      case 10:
        counts1 = 5;
        counts2 = 1;
        break;

      case 11:
        counts1 = 6;
        counts2 = 0;
        break;
      case 12:
        counts1 = 6;
        counts2 = 1;
        break;

      case 13:
        counts1 = 7;
        counts2 = 0;
        break;
      case 14:
        counts1 = 7;
        counts2 = 1;
        break;

      case 15:
        counts1 = 8;
        counts2 = 0;
        break;
      case 16:
        counts1 = 8;
        counts2 = 1;
        break;
      default:
        counts1 = 0;
        counts2 = 8;
        break;
    }
    


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

    bool outputPB0 = calcRhy(counter, counts1);
    digitalWrite(PB0, outputPB0);
    bool outputPB1 = calcRhy(counter, counts2);
    digitalWrite(PB1, outputPB1);
  } else {
    digitalWrite(PB0, false);
    digitalWrite(PB1, false);
  }


  delay(2);  // Short delay to stabilize readings
}
