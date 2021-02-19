/**
   ACS-85-0210
   ATTiny85 Squarewave  VCO with tap delays

   External pin 1       = Reset (not used)
   External pin 2 (PB3) = input 0 freq
   External pin 3 (PB4) = input 1 tap delay/spread
   External pin 4       = GND
   External pin 5 (PB0) = output 0 output
   External pin 6 (PB1) = output 1 output tap 1
   External pin 7 (PB2) = output 2 output tap 2-
   External pin 8       = Vcc

   V 1.0  -  First Version

   Hard to say where this really goes.  Its a VCO like ACS-85-0041, but the tuning word is stored off and replayed
   a little later on.  Kinda like an atari punk console or a shark, it really only works when its in motion.
   This will lead to other drone things in the future where things are just constantly modulated.

   For now though, its just two taps (and the immediate output)

   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
   }434|434|455|500|500|500|522|522|533|.......|222|222|222|233|233|233|255|
   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
                   ^             ^                   ^
                  tap           tap1                tuning  --->

   The Tuning word (note in dds) is stored round robin in the samples array.
   To determine port B, we use the tuning word and then bit 1 and bit 2 are just
   the spread and spread/2 Mod 127.  It it not PWM, we are just flipping bits.
   It sounds better if all bits are set at once.

   The size of 127 is chosen because the math is easy...but there is enough memory for 100 more
   samples.  Alas...not enough to make it a easy 256 ints.

   When mixed, there is some phasing effects.  Consider modulating the phase a bit to make it
   mix no matter what.

   Rob Stave (Rob the fiddler) ccby 2020
*/

//  ATTiny overview
//                        +-\/-+
//                 Reset 1|    |8  VCC
// (pin3) spread A3  PB3 2|    |7  PB2 (pin2) output tap 2
// (pin4) freq   A2  PB4 3|    |6  PB1 (pin1) output tap 1
//                   GND 4|    |5  PB0 (pin0) output
//                        ------


#define  VCO1LOW 200
#define VCO1HIGH 2200



volatile unsigned int Acc1;
volatile unsigned int Note1 = 857;
volatile unsigned int Acc2;
volatile unsigned int Acc3;


volatile byte counter = 0;
volatile unsigned int samples[128];

volatile  byte Spread = 40;

#define NO_NOTE_IF_ZERO true

void setup()
{

  DDRB = B00000111; //set output bits

  noInterrupts(); // disable all interrupts
  // Set up Timer/Counter0 for 20kHz interrupt to output samples.
  TCCR0A = 3 << WGM00;             // Fast PWM
  TCCR0B = 1 << WGM02 | 2 << CS00; // 1/8 prescale
  TIMSK = 1 << OCIE0A;             // Enable compare match, disable overflow
  OCR0A = 49;                      // Divide by 400




  TCCR1 |= (1 << CTC1);  // clear timer on compare match
  TCCR1 |= (1 << CS13) | (1 << CS12) | (1 << CS10); //clock prescaler 40
  OCR1C = 7; // compare match value
  TIMSK |= (1 << OCIE1A); // enable compare match interrupt

  interrupts(); // enable all interrupts
}

// write the data round robi to buffer
ISR(TIMER1_COMPA_vect)
{
  samples[counter] = Note1;
  counter++;
  if (counter > 127) {
    counter = 0;
  }
}



ISR(TIMER0_COMPA_vect)
{
  //Ultimately...portB
  byte result = 0;

  // Calculate first note

  Acc1 = Acc1 + Note1;

  unsigned int upper8bits = (Acc1 >> 8);

  // adjust if no sound needed
  if ( Note1 == 0) {
    upper8bits = 0;
  }

  if (upper8bits > 0x7F ) {
    result |= B00000001;
  }



  // Tap1
  byte localSampleIndex = counter -  (Spread >> 1);

  // based on buffer size of 128...alter math if you change accordingly
  if (localSampleIndex > 127) {
    localSampleIndex = localSampleIndex - 128;
  }
  unsigned int delayNote = samples[localSampleIndex];
  Acc2 = Acc2 + delayNote;



  upper8bits = (Acc2 >> 8 );

  // adjust if no sound needed
  if ( delayNote == 0) {
    upper8bits = 0;
  }

  if (upper8bits > 0x7F) {
    result |= B00000010;
  }

  // Tap2
  localSampleIndex = counter -  Spread;
  if (localSampleIndex > 127) {
    localSampleIndex = localSampleIndex - 128;
  }

  delayNote = samples[localSampleIndex];


  Acc3 = Acc3 + delayNote;

  upper8bits = (Acc3 >> 8);

  // adjust if no sound needed
  if ( delayNote == 0) {
    upper8bits = 0;
  }

  if (upper8bits > 0x7F) {
    result |= B00000100;
  }

  // Write all 3 bits at once.
  PORTB = result;

}

void loop()
{


  while (true)
  {
    unsigned int freqSample = analogRead(A2);
    if ((NO_NOTE_IF_ZERO == true) && (freqSample < 3)) {
      Note1 = 0;
    } else {
      Note1 = map(freqSample, 0, 1023, VCO1LOW, VCO1HIGH);
    }

    Spread = map(analogRead(A3), 0, 1023, 2, 100);
  }
}
