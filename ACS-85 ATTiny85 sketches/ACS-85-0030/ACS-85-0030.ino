/**
   ACS-85-0030
   ATTiny85 4 channel LFO
   1 input and 4 outputs.
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
   will work.

   V 1.0 is not super fast. Considering I was just using LED/LDRs...it was not all that important.

   Each interrupt increments both a counter oscCounter1 and readTimer.
   oscCounter1 is just a counter that rolls over at 255

   PWM is done by checking the value of the channel vs the count.  If the counter is
   less than the value, the bit is 1.

   V 1.1 - Improvements

   Quite frankly, this whole sketch is an antipattern.
   There are ways to get 4 PWM...and well...this is the worst.
   Ideally, you offload pwm to a timer with match compare circuits built in.
   This is the reason why built in PWM is tied to ports.

   But noooooo.....not this whale hunter.

   In this case, Im implementing it in the interrupt which is comsuming so many 
   cycles it really cant even  keep up.  
   Lowering the resolution to 128 steps ( 7 bits ) fixed a lot.
   But you need a lower freq LPF.  Consider 1uf and 10k.  In that case its
   ok...slew rate is high...but works for certain aspects.

   If you REALLY need 4 PWM...use a ATTINY84 at 16 mhz or just 2 85s with
   a better rate.


   Speed is 8,000,000 / ( 8 prescaler * (4+1)) so 200k
   But the counter is 255 steps...so its really like 784 hz
   
   I dropped the resolution by incrementing by 128.  That helped, but again, 
   this is not super good for fast things.  Slow slews...fine.
   With that Im getting 900hz pwm.  You would think it would double the PWM,
   but the interrupt is doing way more work than it needed to for a regular
   pwm situation and those cycles have a higher priority that "loop".

  Still, you look at it as a black box that does what it does...np

   Rob Stave (Rob the fiddler) CCBY 2019
*/

//  ATTiny overview
//                         +-\/-+
//                  Reset 1|    |8  VCC
// (pin3) in      A3  PB3 2|    |7  PB2 (pin2) out lfo
// (pin4) out LFO A2  PB4 3|    |6  PB1 (pin1) out lfo
//                    GND 4|    |5  PB0 (pin0) out lfo
//                         ------





uint8_t values [] =  { 0, 0, 0, 0};

// Attempt to deref and speed up the interrupt with bytes vs array.
// did not seem to make much difference
volatile uint8_t values1 =  0;
volatile uint8_t values2 =  0;
volatile uint8_t values3 =  0;
volatile uint8_t values4 =  0;


// Channel State
// State of each pin.
// - Speed:        Ticks for PWM. The higher the number of ticks, the longer the period
// - SilenceCount: Time between triangle pulses.  Basically a countdown
// - Direction:    Count up/down for waveform
typedef struct ChannelState {
  uint8_t speed;
  uint8_t ticks;
  long silenceCounter;
  bool direction;
};


// Initialize with a random speed and silence count.  Playing with these values
// will change the characteristics.  For example, the 1 to 12 is basically the rise
// and fall speed.  1 means there will be 255/1 ticks to go from 0 to max whereas
// 12 means 255/12 ticks ( much faster).
//
//  the 80-2000 is the silence ticks between swells.

#define INIT_SILENCE_LONG 2500
#define INIT_SILENCE_SHORT 100

// Highest and lowest speed.
#define MAX_LOOP_COUNT 300
#define MIN_LOOP_COUNT 1

// How often to read analog pin.  This is supposed to be kinda slow, but if you need
// a snappier response to changes in the pin...decrement this
#define ANALOG_LOOP_COUNT 6000

ChannelState channel[] = {
  {random(1, 10), 0, random(INIT_SILENCE_SHORT, INIT_SILENCE_LONG), true },
  {random(1, 10), 0, random(INIT_SILENCE_SHORT, INIT_SILENCE_LONG), true },
  {random(1, 10), 0, random(INIT_SILENCE_SHORT, INIT_SILENCE_LONG), true },
  {random(1, 10), 0, random(INIT_SILENCE_SHORT, INIT_SILENCE_LONG), true }
};


// the setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00010111;  //set output bits (and input  pb3)

  // initialize timer1
  noInterrupts();           // disable all interrupts

  TCCR0A = 0;
  TCCR0B = 0;

  TCCR0A |= (1 << WGM01); //Start timer 1 in CTC mode Table 11.5
  OCR0A = 4; //CTC Compare value
  TCCR0B |= (1 << CS01); // Prescaler =8 Table 11.6
  TIMSK |= (1 << OCIE0A); //Enable CTC interrupt see 13.3.6
  interrupts();             // enable all interrupts
}


ISR(TIMER0_COMPA_vect) // timer compare interrupt service routine
{
  static uint8_t oscCounter1;

  oscCounter1 = oscCounter1 + 2;

  byte output = B00000000;

  if ( oscCounter1 < values1) {
    output = output | B00000001;
  }

  if ( oscCounter1 < values2) {
    output = output | B00000010;
  }

  if ( oscCounter1 < values3) {
    output = output | B00000100;
  }

  if ( oscCounter1 < values4) {
    output = output | B00010000;
  }

  PORTB = output;

}

uint8_t CalculateChannel(   int i  ) {

  if (channel[i].silenceCounter > 0) {
    channel[i].silenceCounter--;
    values[i] = 0;
  }  else {

    channel[i].ticks++;

    if (channel[i].ticks > channel[i].speed) {
      channel[i].ticks = 0;

      if (channel[i].direction == true) {

        values[i] = min(values[i] + 1, 255);
        if (values[i] == 255) {
          channel[i].direction = false;
        }
      } else {
        values[i] = max(values[i] - 1, 0);
        if (values[i] == 0) {
          channel[i].direction = true;
          channel[i].silenceCounter = random(INIT_SILENCE_SHORT, INIT_SILENCE_LONG);
          channel[i].speed = random(1, 8);
        }
      }
    }
  }

  return values[i];
}



void loop() {

  uint16_t initialSpeed = MIN_LOOP_COUNT;
  uint16_t checkPot = 0;
  uint16_t readTimer = 0;
  randomSeed(analogRead(A3));

  while (true) {

    readTimer++;
    // read analoge value from time to time.  Ideally we are not changing this much, but you can
    // decrease this count if you want to read the pot more often
    checkPot++;
    if (checkPot > ANALOG_LOOP_COUNT) {
      uint16_t osc1_t = analogRead(A3);
      initialSpeed = map(osc1_t, 0, 1023, MIN_LOOP_COUNT, MAX_LOOP_COUNT);
      checkPot = 0;
    }

    if (readTimer > initialSpeed) {
      values1 = CalculateChannel( 0);
      values2 = CalculateChannel( 1);
      values3 = CalculateChannel( 2);
      values4 = CalculateChannel( 3);

      readTimer = 0;
    }
  }

}