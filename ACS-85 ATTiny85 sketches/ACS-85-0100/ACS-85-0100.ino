/**
   ACS-85-0100 ATTiny85 - blip

   External pin 2 = Pitch
   External pin 3 = Blip Length
   External pin 5 = Blip sound
   External pin 6 = Noise
   External pin 7 = Trigger

    ATTiny overview
                            +-\/-+
                     Reset 1|    |8  VCC
         (pin3) pitch  PB3 2|    |7  PB2 trigger
         (pin4) length PB4 3|    |6  PB1 (pin1)  noise
                       GND 4|    |5  PB0 (pin0)  blip
                            ------

   Description:
   Does a blip sound on one pin and noise on the other
   The two pins can be summed via resistors (or op amp)...or not.  Up to you

   There is a FLAG that allows you to cycle through all the sounds or just pick one.
   If you were so inclined. ( Hardcoded of course...there just is not enougn inputs
   to do everything)

   To minimize parts, consider locking down a speed/ length as well and not using the pots.

   V 1.0  -  First Version

   Rob Stave (Rob the Fiddler) CCBY 2019

*/

// Different sounds.  These are not samples, but algorithms.
//  Its just a start, feel free to mix/match and create your own.

#define BLIP 0     // arp up
#define BLIZ 1     // arp between 2 notes
#define BLOOP 2    // arp down
#define BEEP 3     // just a note
#define BLIFF 4    // sound and noise
#define BLUMP 5    // kinda the same
#define BLOOEEOO 6 // ARP up and down
#define BIZEEP 7   // noise to beep

#define LAST_SOUND 7 // Set to highest one

// SET this to true to rotate through the sounds, otherwise pick one you like an set this to false
#define SINGLE_SOUND 0
#define ROTATE_SOUND 1
#define RANDOM_SOUND 2
// note if you want to SELECT a sound...See ACS-85-0101

#define CHOOSE_SOUND SINGLE_SOUND
// if you want just one sound, set it here with ABOVE as SINGLE_SOUND
volatile int sound = BLOOEEOO;

// Debounce time
#define DEBOUNCE 30

volatile unsigned int Acc1;                      // main freq
volatile unsigned int StartNote = 800;           // tuning word
volatile unsigned int noteCounter = 0;           // tuning word
volatile unsigned int noteLengthMaxMillis = 500; // tuning word

volatile boolean isRunning = false;
volatile boolean setTrigger = false;
volatile byte multiplier = 1;

volatile unsigned int lfsr = 1;

// the setup function runs once when you press reset or power the board
void setup()
{

  DDRB = B00000011; // Set port B output bits

  // initialize timer
  noInterrupts(); // disable all interrupts

  TCCR0A = 0;
  TCCR0B = 0;
  TCCR0A |= (1 << WGM01); // Start timer in CTC mode Table 11.5

  OCR0A = 5; // CTC Compare value...this is fairly arbitrary and you can change, but have to adjust math.

  TCCR0B |= (1 << CS00) | (1 << CS01); // Prescaler =64 Table 11.6
  TIMSK |= (1 << OCIE0A);              // Enable CTC interrupt see 13.3.6

  // start the timer, prescaler
  TCCR1 = (1 << CTC1) | (7 << CS10); // CTC  mode, div64
  OCR1C = 0.001 * F_CPU / 64 - 1;    // 1ms, F_CPU @16MHz, div64
  TIMSK |= (1 << OCIE1A);

  interrupts(); // enable all interrupts
}

// Use the second timer as our "timer"
volatile unsigned long milliseconds;

// Set second timer to implement "millis"
ISR(TIMER1_COMPA_vect)
{
  milliseconds++;
}

unsigned long millisTimer1()
{
  return milliseconds;
}

// Used for the the LFSR(noise)
void clockLfsr()
{

  // calculate new state
  boolean outputBit = bitRead(lfsr, 10) ^ bitRead(lfsr, 12) ^ bitRead(lfsr, 13) ^ bitRead(lfsr, 15);
  lfsr = lfsr << 1;
  lfsr |= outputBit;
}

ISR(TIMER0_COMPA_vect) // timer compare interrupt service routine
{
  if (isRunning == true)
  {

    // output the noise bit first
    //  This is a freebie bit...but perhaps you can use it below
    boolean outputBit = bitRead(lfsr, 10);
    if (outputBit == true)
    {
      bitSet(PORTB, 1);
    }
    else
    {
      bitClear(PORTB, 1);
    }

    if (sound == BEEP)
    {

      //  BEEP just a note
      Acc1 = Acc1 + StartNote;
    }
    else if (sound == BLIP)
    {

      //  Blip... Arp Up sound
      //  The multiplier is a vaule that goes from 
      //  0 to 6 over the course of the note.
      //  In this case it will arp up
      Acc1 = Acc1 + StartNote + StartNote * multiplier;
    }
    else if (sound == BLIZ)
    {

      // Toggle between note and octave up
      Acc1 = Acc1 + StartNote + StartNote * (multiplier % 2);
    }
    else if (sound == BLOOP)
    {

      //  Arp down
      Acc1 = Acc1 + (StartNote >> (multiplier % 3));
    }
    else if (sound == BLIFF)
    {

      // oct up if noise bit is true

      Acc1 = Acc1 + StartNote + StartNote * outputBit;
    }
    else if (sound == BLUMP)
    {

      // oct up if noise bit is true

      int mod = 0;
      if (multiplier % 2 == 0)
      {
        if (outputBit == true)
        {
          mod = StartNote;
        }
      }
      Acc1 = Acc1 + StartNote + mod;
    }
    else if (sound == BLOOEEOO)
    {

      int mod = 0;
      if (multiplier == 2)
      {
        mod = StartNote << 1;
      }
      if (multiplier == 1 || multiplier == 3)
      {
        mod = StartNote;
      }
      Acc1 = Acc1 + StartNote + mod;
    }
    else if (sound == BIZEEP)
    {

      // tuned noise for the first 1/3 of the note
      int mod = 0;
      if (multiplier < 2)
      {
        if (outputBit == true)
        {
          mod = StartNote;
        }
      }

      Acc1 = Acc1 + StartNote + mod;
    }
    else
    {
      // do nothing
    }

    // Write bit for sound
    uint8_t result1 = (Acc1 >> 8) & 0x80;
    if (result1 > 0)
    {
      bitSet(PORTB, 0);
    }
    else
    {
      bitClear(PORTB, 0);
    }
  }
  else
  {
    bitClear(PORTB, 0);
    bitClear(PORTB, 1);
  }
}

void checkIfRotate()
{
  if (CHOOSE_SOUND == SINGLE_SOUND)
  {
    return;
  }
  if (CHOOSE_SOUND == ROTATE_SOUND)
  {

    sound = sound + 1;
    if (sound > LAST_SOUND)
    {
      sound = 0;
    }
    return;
  }

  if (CHOOSE_SOUND == RANDOM_SOUND)
  {
    // only really picks 0 - 7
    sound = lfsr & B00000111;
  }
}

void loop()
{

  unsigned long previousMillis = 0;

  // no need to check speed and LFO all that quickly
  // so we space that out a bit
  byte sensorCounter = 0;
  byte sensorLimit = 500;
  byte checkState = 0;

  int buttonState;                    // the current reading from the input pin
  int lastButtonState = LOW;          // the previous reading from the input pin
  unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
  unsigned long debounceDelay = 50;   // the debounce time; increase if the output flickers
  while (true)
  {
    // get frequency
    clockLfsr();

    // Debound stuff
    unsigned long currentMillis = millisTimer1();

    int reading = digitalRead(PB2);

    // check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH), and you've waited long enough
    // since the last press to ignore any noise:

    // If the switch changed, due to noise or pressing:
    if (reading != lastButtonState)
    {
      // reset the debouncing timer
      lastDebounceTime = currentMillis;
    }

    if ((currentMillis - lastDebounceTime) > debounceDelay)
    {
      // whatever the reading is at, it's been there for longer than the debounce
      // delay, so take it as the actual current state:

      // if the button state has changed:
      if (reading != buttonState)
      {
        buttonState = reading;

        // only toggle the trigger if the new button state is HIGH
        if (buttonState == HIGH)
        {
          isRunning = false;
          setTrigger = true;
          noteCounter = 0;
        }
      }
    }
    lastButtonState = reading;

    // Check sensors too.  No need to check every loop.  Just every [sensorLimit] counts.
    // If you want more responsiveness to the pots, you can decrease, at the expense of more
    // clock cycles reading pots.
    if (sensorCounter > sensorLimit)
    {
      if (checkState % 2 == 0)
      {
        int sample = analogRead(A3);
        StartNote = map(sample, 0, 1023, 2800, 300);
      }
      else
      {
        int sample3 = analogRead(A2);
        noteLengthMaxMillis = map(sample3, 0, 1023, 40, 500);
      }

      sensorCounter = 0;
      checkState++;
    }
    sensorCounter++;

    if (setTrigger == true)
    {

      checkIfRotate(); // determine if need to rotate or just use one sound
      // Kick off note.  Store off previous millis to track length
      isRunning = true;
      Acc1 = 0;
      setTrigger = false;
      noteCounter = 0;
      previousMillis = currentMillis;
    }

    if (isRunning == true)
    {
      noteCounter = currentMillis - previousMillis;

      // The note will last as long as
      // noteCounter > noteLengthMaxMillis.  However, I have added below a "section"
      // as well.  Maybe a mini ASDR ?  We are just spliting the note in 6 sections.
      // so that we can do things like bitshift the accumulator step to get arp sounds.

      multiplier = map(noteCounter, 0, noteLengthMaxMillis, 0, 6);

      if (noteCounter >= noteLengthMaxMillis)
      {
        // Note is finished
        isRunning = false;
        setTrigger = false;
        previousMillis = currentMillis;
      }
    }
    else
    {
      setTrigger = false;
      noteCounter = 0;
    }
  }
}
