
/**
   ACS-85-0621
   ATTiny85 Timed Gate

   Takes a logical input and Ands it with a timed gate
   When the trigger starts the gate it goes for a length defined by A3

   For the length of that gate, PB0 is true and PB1 reflects the output of pin 7.

   if the trigger is high longer than the timer, then its just on...but the gate
   will diable when the trigger does.

   Best for Logical LFO and gates.  Prob not super good for audio as the value 
   of the input is written to the output in a loop. 
   An Optimization would be to set the output pin to the value of the input in the interrupt.
   (Actually I tried it and had issues so I gave up..its fine for low freqs)
   

   External pin 1       = Reset (not used)
   External pin 2 (PB3) = input Gate Length
   External pin 3 (PB4) = input Trigger Gate
   External pin 4       = GND
   External pin 5 (PB0) = output GATE
   External pin 6 (PB1) = output GATED Input
   External pin 7 (PB2) = Input 
   External pin 8       = Vcc
   V 1.0  -  First Version

   Rob Stave (Rob the fiddler) ccby 2024
*/


//  ATTiny overview
//                           +-\/-+
//                    Reset 1|    |8  VCC
//     (pin3) Length 2  PB3 2|    |7  PB2 (pin2) input
//     (pin4) Trigger 3 PB4 3|    |6  PB1 (pin1) Gated output
//                      GND 4|    |5  PB0 (pin0) Gate Out
//                           ------

// Configuration values

#define GATELENGTHMIN 0     // min time of the Gate
#define GATELENGTHMAX 2000  // max time of the Gate

#define GATEPIN PB0
#define OUTPIN PB1
#define INPIN PB2
#define TRIGGER PB4


// the setup function runs once when you press reset or power the board
void setup() {
  DDRB = B00000011;  //set output bits
}

void loop() {

  unsigned long gateTime = 0;  // length of delay
  unsigned long currentMillis; // now
  unsigned long startGateTime;  // time in millis when gate started
  // variables
  boolean gateRunning = false;
    bool trig;
    byte loopCount  = 0;
  bool input;

  //initialize things a bit
  digitalWrite(OUTPIN, false);
  digitalWrite(GATEPIN, false);

  while (true) {
    loopCount++;

    trig = digitalRead(TRIGGER);
    input = digitalRead(INPIN);

    if (gateRunning == false && trig) {
      gateRunning = true;
      startGateTime = millis();
      gateTime = map(analogRead(A3), 0, 1023, GATELENGTHMIN, GATELENGTHMAX);
    }

 

    currentMillis = millis();


    if (gateRunning) {

      digitalWrite(OUTPIN, input);
      digitalWrite(GATEPIN, true);

      if ((currentMillis - startGateTime) >= gateTime) {

        gateRunning = false;
        digitalWrite(OUTPIN, false);
        digitalWrite(GATEPIN, false);
      }
    }
  }
}