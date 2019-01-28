/**
   ACS-85-0030
   ATTiny85 4 channel LFO
   2 inputs and 3 outputs.
   External pin 1       = Reset (not used)
   External pin 2 (PB3) = input 0
   External pin 3 (PB4) = output  LFO PWM
   External pin 4       = GND
   External pin 5 (PB0) = output  LFO PWM
   External pin 6 (PB1) = output  LFO PWM
   External pin 7 (PB2) = output  LFO PWM
   External pin 8       = Vcc

   V 1.0  -  First Version

   Note: This sketch has been written specifically for ATTINY85 and not Arduino uno  but it prob
   will work
   Rob Stave (Rob the fiddler) CCBY 2019
*/

//  ATTiny overview
//                         +-\/-+
//                  Reset 1|    |8  VCC
// (pin3) in      A3  PB3 2|    |7  PB2 (pin2) out lfo
// (pin4) out LFO A2  PB4 3|    |6  PB1 (pin1) out lfo
//                    GND 4|    |5  PB0 (pin0) out lfo
//                         ------

// Highest and lowest speed.
#define MAX_LOOP_COUNT 4000
#define MIN_LOOP_COUNT 200

// How often to read analog pin.  This is supposed to be kinda slow, but if you need
// a snappier response to changes in the pin...decrement this
#define ANALOG_LOOP_COUNT 1200

volatile uint8_t oscCounter1 = 0;  // Byte counter used to set PWM widdth

// another counter used to regulate the loop processing
//  This is prob not the best way to do it...but..eh...LFO right
volatile uint16_t readTimer = 0; 

volatile uint8_t values [] =  { 0, 0, 0, 0};

// Channel State
// State of each pin.  
// Speed...ticks for PWM. The higher the number of ticks, the longer the period
// SilenceCount.  Time between triangle pulses.  Basically a countdown
//  Count up/down for waveform
typedef struct ChannelState {
  uint8_t speed;
  long silenceCounter;
  bool direction;
};

// using global memory.  You could use local variables in loop as well if your
// good with pointers.

// Initialize with a random speed and silence count
ChannelState channel[] = {
  {random(1, 12), random(30, 250), true },
  {random(1, 12), random(30, 250), true },
  {random(1, 12), random(30, 250), true },
  {random(1, 12), random(30, 250), true }
};


// the setup function runs once when you press reset or power the board
void setup() {

  DDRB = B11110111;  //set output bits (and input

  // initialize timer1
  noInterrupts();           // disable all interrupts

  TCCR0A = 0;
  TCCR0B = 0;

  TCCR0A |= (1 << WGM01); //Start timer 1 in CTC mode Table 11.5

  OCR0A = 29; //CTC Compare value

  TCCR0B |= (1 << CS01); // Prescaler =8 Table 11.6

  // TCCR0A |=(1<<COM0A1); //Timer0 in toggle mode Table 11.2
  TIMSK |= (1 << OCIE0A); //Enable CTC interrupt see 13.3.6
  interrupts();             // enable all interrupts
}


ISR(TIMER0_COMPA_vect) // timer compare interrupt service routine
{

  readTimer++;

  oscCounter1++;
  byte output = B00000000;

  if ( oscCounter1 < values[0]) {
    bitSet(PORTB, 0);
  } else {
    bitClear(PORTB, 0);
  }
  if ( oscCounter1 < values[1]) {
    bitSet(PORTB, 1);
  } else {
    bitClear(PORTB, 1);
  }
  if ( oscCounter1 < values[2]) {
    bitSet(PORTB, 2);
  } else {
    bitClear(PORTB, 2);
  }

  if ( oscCounter1 < values[3]) {
    bitSet(PORTB, 4);
  } else {
    bitClear(PORTB, 4);
  }
}


void CalculateChannel(   int i  ) {

  if (channel[i].silenceCounter > 0) {
    channel[i].silenceCounter--;
  }  else {
    //long v = values[i];
    if (channel[i].direction == true) {
      values[i] = min(values[i] + channel[i].speed, 255);
      if (values[i] == 255) {
        channel[i].direction = false;

      }
    } else {
      values[i] = max(values[i] - channel[i].speed, 0);
      if (values[i] == 0) {
        channel[i].direction = true;
        channel[i].silenceCounter = random(30, 200);
        channel[i].speed = random(1, 12);
      }
    }

  }


}

void loop() {

  long initialSpeed = MIN_LOOP_COUNT;
  long checkPot = 0;

  randomSeed(analogRead(A3));

  
  while (true) {

    // read analoge value from time to time.  Ideally we are not changing this much, but you can
    // decrease this count if you want to read the pot more often
    checkPot++;
    if (checkPot > ANALOG_LOOP_COUNT) {
      uint16_t osc1_t = analogRead(A3);
      initialSpeed = map(osc1_t, 0, 1023, MIN_LOOP_COUNT, MAX_LOOP_COUNT);
      checkPot = 0;
    }


    if (readTimer > initialSpeed) {

      CalculateChannel( 0);
      CalculateChannel( 1);
      CalculateChannel( 2);
      CalculateChannel( 3);

      readTimer = 0;
    }


  }

}
