/**
   ACS-85-0042
   ATTiny85 Squarewave  VCO  with chip tune sound

   External pin 1       = Reset (not used)
   External pin 2 (PB3) = input 0 freq
   External pin 3 (PB4) = input 1 selection
   External pin 4       = GND
   External pin 5 (PB0) = output 0 output
   External pin 6 (PB1) = output 1 output chip
   External pin 7 (PB2) = output 2 none
   External pin 8       = Vcc

   V 1.0  -  First Version

   Chiptune sound really is set up for 8Mhz ATTINY.  If you run this on 16mhz or more, you may need
   to mess with the counters.

   Basically, it does a lot of arping to get that sound.
   There are several algorithms.  See below

   Improvements you could make:
   pin2 is an interrupt pin, so you could use it as a sync.
   Just zero out effectModulator count on trigger up.
   
   Rob Stave (Rob the fiddler) ccby 2020
*/

//  ATTiny overview
//                       +-\/-+
//                 Reset 1|    |8  VCC
// (pin3) select A3  PB3 2|    |7  PB2 (pin2) none
// (pin4) freq   A2  PB4 3|    |6  PB1 (pin1) vco chiptune
//                   GND 4|    |5  PB0 (pin0) vco output
//                        ------

volatile unsigned int ChipAcc;
volatile unsigned int Acc;
volatile unsigned int Note = 857; // starting note...will get blasted in the loop
volatile unsigned int SelectionMode = 50;


volatile unsigned int effectsMod = 0;

// Pretty important number.  If you make it bigger, the arp speed goes down.
// the Levels split up each arp period.

// Example, a ramp up appeggio could be
//   Note   Note   Note   Note   Oct    Oct    Oct2   Oct2
// |------|------|------|------|------|------|------|------|
//
#define EFFECTS_MAX_COUNT 2500
#define EFFECTS_LVL_1  EFFECTS_MAX_COUNT/8
#define EFFECTS_LVL_2  EFFECTS_MAX_COUNT/8 * 2
#define EFFECTS_LVL_3  EFFECTS_MAX_COUNT/8 * 3
#define EFFECTS_LVL_4  EFFECTS_MAX_COUNT/8 * 4
#define EFFECTS_LVL_5  EFFECTS_MAX_COUNT/8 * 5
#define EFFECTS_LVL_6  EFFECTS_MAX_COUNT/8 * 6
#define EFFECTS_LVL_7  EFFECTS_MAX_COUNT/8 * 7



void setup()
{

  DDRB = B00000111; //set output bits

  noInterrupts(); // disable all interrupts
  // Set up Timer/Counter0 for 20kHz interrupt to output samples.
  TCCR0A = 3 << WGM00;             // Fast PWM
  TCCR0B = 1 << WGM02 | 2 << CS00; // 1/8 prescale
  TIMSK = 1 << OCIE0A;             // Enable compare match, disable overflow
  OCR0A = 49;                      // Divide by 400

  interrupts(); // enable all interrupts
}

void loop()
{

  // Used for sample averaging
  int f1Sample[4] = {0, 0, 0, 0};

  byte counter = 0;

  int VCO1LOW = 200;
  int VCO1HIGH = 2200;

  while (true)
  {

    // Average over 4 samples.  Not super important...you can ditch if you feel the need
    f1Sample[counter] = analogRead(A2);
    unsigned int osc1_t = (f1Sample[0] + f1Sample[1] + f1Sample[2] + f1Sample[3]) >> 2;
    Note = map(osc1_t, 0, 1023, VCO1LOW, VCO1HIGH);

    // only check this 1 out of 4 times in loop
    if (counter == 0) {
      SelectionMode = map(analogRead(A3), 0, 1023, 0, 100);
    }

    counter++;
    if (counter >= 4) {
      counter = 0;
    }

  }
}

/***
   Changes the none based on effectModulator (a counter in the DDS loop) and a selected Mode
*/
int calcNote(int currentNote, int effectModulator, int mode) {

  int newNote = currentNote;

  if (mode < 17) {
    return arpUp( currentNote, effectModulator);
  }

  if (mode < 33) {
    return arpUp2( currentNote, effectModulator);
  }

  if (mode < 50) {
    return arpDown( currentNote, effectModulator);
  }

  if (mode < 67) {
    return toggle( currentNote, effectModulator);
  }

  if (mode < 84) {
    return toggle2( currentNote, effectModulator);
  }


  return ramp( currentNote, effectModulator);

}

int arpUp( int currentNote, int effectModulator) {

  int newNote = currentNote;

  if (effectModulator > EFFECTS_LVL_4)  {
    newNote = newNote + newNote;
    if (effectModulator > EFFECTS_LVL_6)  {
      newNote = newNote + newNote;
      if (effectModulator > EFFECTS_LVL_7) {
        newNote = newNote + newNote;
      }
    }
  }
  return newNote;
}

int arpUp2( int currentNote, int effectModulator) {

  int newNote = currentNote;

  if (effectModulator > EFFECTS_LVL_4)  {
    newNote = newNote + newNote;

    if (effectModulator > EFFECTS_LVL_7) {
      newNote = newNote + newNote << 1;
    }

  }
  return newNote;
}

// Toggle between note and octave up
int toggle( int currentNote, int effectModulator) {

  int newNote = currentNote;

  if (effectModulator > EFFECTS_LVL_6)  {
    return   newNote + newNote;
  }

  if (effectModulator > EFFECTS_LVL_4) {
    return newNote;
  }

  if (effectModulator > EFFECTS_LVL_2)  {
    return   newNote + newNote;
  }

  return newNote;
}

// Faster toggle twist at the last part
int toggle2( int currentNote, int effectModulator) {

  int newNote = currentNote;

  if (effectModulator > EFFECTS_LVL_7)  {
    return   newNote + newNote;
  }

  if (effectModulator > EFFECTS_LVL_6) {
    return newNote;
  }

  if (effectModulator > EFFECTS_LVL_5)  {
    return   newNote + newNote;
  }

  if (effectModulator > EFFECTS_LVL_4) {
    return newNote;
  }

  if (effectModulator > EFFECTS_LVL_3)  {
    return   newNote + newNote;
  }

  if (effectModulator > EFFECTS_LVL_2)  {
    return newNote;
  }


  return newNote + newNote + newNote;
}

// Meh...kinda rampy
int ramp( int currentNote, int effectModulator) {

  int newNote = currentNote;
  if (effectModulator > EFFECTS_LVL_5)  {
    int x = effectModulator % (currentNote );
    return newNote + x;
  }
  return newNote;

}


int arpDown( int currentNote, int effectModulator) {

  int newNote = currentNote;
  if (effectModulator > EFFECTS_LVL_4)
  {
    if ( effectModulator > EFFECTS_LVL_6) {
      return   newNote >> 1;
    }
    return  newNote - ( newNote >> 2);
  }
  return newNote;
}


ISR(TIMER0_COMPA_vect)
{

  effectsMod++;
  if (effectsMod > EFFECTS_MAX_COUNT) {
    effectsMod = 0;
  }

  Acc = Acc + Note;
  unsigned int upper8bits = (Acc >> 8);

  if (upper8bits > 0x7F)  {
    bitSet(PORTB, 0);
  } else {
    bitClear(PORTB, 0);
  }


  ChipAcc = ChipAcc + calcNote(Note, effectsMod, SelectionMode);


  upper8bits = (ChipAcc >> 8);

  if (upper8bits > 0x7F)  {
    bitSet(PORTB, 1);
  }  else  {
    bitClear(PORTB, 1);
  }

}
