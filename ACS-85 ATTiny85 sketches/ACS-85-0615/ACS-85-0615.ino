/**
   ACS-85-0615
   ATTiny85  Clocked prob
     
   The knob is just a value that is compared to the LFSR
   if its low then the check is the bottom 8 bits vs the value.

   So a zero is pretty much always lower that the LFSR and is not a drop.

   There is no debouncing, consider a lpf in from of the clock.  it does not take much

   The CV adds to the value and  it rotates at 255 so 50% + 20% = 70%...but it will wrap.
   This means that if your only interested in using the cv...turn the knob to zero.

   Rob Stave (Rob the fiddler) ccby 2023
*/

//  ATTiny overview
//                           +-\/-+
//                    Reset 1|    |8  VCC
//   (pin4) mode   A3   PB3 2|    |7  PB2 (pin2) clock
//   (pin4) cv mod A2   PB4 3|    |6  PB1 (pin1) clock out a
//                      GND 4|    |5  PB0 (pin0) clock out b
//                           ------

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

const int clockInt = 0;  // digital pin 2 is now interrupt 0
volatile unsigned int lfsr = 11;

volatile bool triggerDetected = false;

// the setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00000011;  //set output bits
  noInterrupts();    // disable all interrupts
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);  // Enable PWM output pin
  pinMode(2, INPUT);
  attachInterrupt(clockInt, clockCounter, RISING);
  interrupts();  // enable all interrupts
}

void clockCounter()  // called by interrupt
{
  triggerDetected = true;
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

  boolean clockTriggered = false;

  sbi(ADCSRA, ADSC);  //start next convert

  while (true) {

    if (triggerDetected == true) {
      clockTriggered = true;
      triggerDetected = false;

      uint8_t sample = lfsr & B11111111;

      // prob is compared against the pot and cv added.  So if there is nothing plugged in, its just the knob
      // and vice versa.  if both are at a value, you will see a wrap around effect.
      
      uint8_t compare = prob + mod;
      if (sample >= compare) {
        digitalWrite(1, LOW);
        digitalWrite(0, HIGH);
      } else {
        digitalWrite(1, HIGH);
        digitalWrite(0, LOW);
      }
    }

    clockLfsr();

    if (clockTriggered && digitalRead(2) == LOW) {
      clockTriggered = false;
      digitalWrite(0, LOW);
      digitalWrite(1, LOW);
    }

    // read pots
    if (!(ADCSRA & 64)) {
      if (MUX == 2) {
        int x = ((ADCL + (ADCH << 8)));
        mod = map(x, 0, 1023, 0, 255);
      }
      if (MUX == 3) {
        int y = ((ADCL + (ADCH << 8)));
        prob = map(y, 0, 1023, 0, 255);
      }
      MUX++;
      if (MUX == 4) MUX = 2;
      ADMUX = MUX;        //Select MUX
      sbi(ADCSRA, ADSC);  //start next conversation
    }
  }
}