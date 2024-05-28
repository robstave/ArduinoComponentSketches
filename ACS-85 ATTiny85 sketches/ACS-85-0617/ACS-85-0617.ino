/**
   ACS-85-0617
   ATTiny85  Probability Drop Gate 
     
  If the drop pin is low, this is just a 100 passthrough.
  If the pin is high, then the top pin follows the input put with
  a probability of dropping.

  The other pin is high on the drop.

  This is not too particularly different than  ACS-85-0615

   Rob Stave (Rob the fiddler) ccby 2024
*/

//  ATTiny overview
//                           +-\/-+
//                    Reset 1|    |8  VCC
//   (pin4) Prob   A3   PB3 2|    |7  PB2 (pin2) clock
//   (pin4) Drop        PB4 3|    |6  PB1 (pin1) clock out a
//                      GND 4|    |5  PB0 (pin0) clock out b
//                           ------

#define outputPin PB1
#define outputPinNot PB0

const int clockInt = 0;  // digital pin 2 is now interrupt 0
volatile unsigned int lfsr = 11;

volatile bool riseDetected = false;

// the setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00000011;  //set output bits
  noInterrupts();    // disable all interrupts
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);   
  pinMode(2, INPUT);
  attachInterrupt(clockInt, clockCounter, RISING);
  interrupts();  // enable all interrupts
}

void clockCounter()  // called by interrupt
{
  riseDetected = true;
}

// LFSR for pseudorandom numbers
void clockLfsr() {

  //calculate new state
  boolean outputBit = bitRead(lfsr, 10) ^ bitRead(lfsr, 12)
                      ^ bitRead(lfsr, 13) ^ bitRead(lfsr, 15);
  lfsr = lfsr << 1;
  lfsr |= outputBit;
}

void loop() {

  uint8_t mod = 0;
  uint8_t prob = 0;
  uint8_t MUX = 2;
  bool doDrop = false;

  boolean doOutput = false;

 
  while (true) {

    doDrop = digitalRead(PB4);

    if (doDrop) {
      prob = map(analogRead(A3), 0, 1023, 0, 255);
    }


    if (riseDetected == true) {
      doOutput = true;
      riseDetected = false;

      uint8_t sample = lfsr & B11111111;
 
      
      if (doDrop) { // its 
        if (prob > sample) {
          digitalWrite(outputPin, LOW);
          digitalWrite(outputPinNot, HIGH);
        } else {
          digitalWrite(outputPin, HIGH);
          digitalWrite(outputPinNot, LOW);
        }
      } else {
          digitalWrite(outputPin, HIGH);
          digitalWrite(outputPinNot, LOW);
      }
    }
 
    clockLfsr();

    if (doOutput && digitalRead(2) == LOW) {
      doOutput = false;
      digitalWrite(outputPin, LOW);
      digitalWrite(outputPinNot, LOW);
    }

    
  }
}