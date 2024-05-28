
/**
   ACS-85-0618
   ATTiny85 clock divider

    divide the clock by the count of the pot.
    Divide will be a Gate Version with 50% on and 50% off
    Trigger will be the same as divide, but just is a trigger on the rising clock
    
    So say the divide is 3
    Clock  : 1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0
    Trig   : 1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0
    Divide : 1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0

    reset resets the divisions
   

   External pin 1       = Reset (not used)
   External pin 2 (PB3) = input DivBy (A3)
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
//     (pin3) divide  2 PB3 2|    |7  PB2 (pin2) clock
//     (pin4) reset   3 PB4 3|    |6  PB1 (pin1) output B
//                      GND 4|    |5  PB0 (pin0) output A
//                           ------

// Configuration values

#define MINCOUNT 2   
#define MAXCOUNT 9   

#define DIVIDEPIN PB1
#define TRIGGERPIN PB0

volatile byte divBy = MINCOUNT;
volatile unsigned int counter = 0;


const int clockInt = 0;  // digital pin 2 is now interrupt 0

// the setup function runs once when you press reset or power the board
void setup() {
  DDRB = B00000011;  //set output bits
 // attachInterrupt(clockInt, clockCounterRise, RISING);
  attachInterrupt(clockInt, clockCounter, CHANGE);
}


bool calculatePinValue(byte divideBy, uint16_t cnt) {
    return cnt  <= (divideBy);

}

bool calculatePinTrigger(byte divideBy, uint16_t cnt) {
    return cnt   == 1;
}

void clockCounter()  // called by interrupt
{
  counter++;
  if ( counter == 1 && !digitalRead(2)){
    counter = 2;
  }
  digitalWrite(DIVIDEPIN, calculatePinValue(divBy, counter));
  digitalWrite(TRIGGERPIN, calculatePinTrigger(divBy, counter));
  
  if (counter >=  divBy*2 ) {
    counter = 0;
  }
}

void loop() {

  bool resetActive = false;
  bool resetHigh = false;
  while (true) {

    divBy = map(analogRead(A3), 0, 1023, MINCOUNT, MAXCOUNT);
    resetHigh = digitalRead(4);

    // latch the reset
    if (resetHigh && !resetActive) {
      counter = 50;
      resetActive = true;
    }

    if (!resetHigh && resetActive) {
      resetActive = false;
    }
  }
}