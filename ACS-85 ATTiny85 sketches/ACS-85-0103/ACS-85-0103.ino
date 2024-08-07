/**
   ACS-85-0103 ATTiny85 - space drum 2

   External pin 2 = Drum sound out3
   External pin 3 = Drum sound out2
   External pin 5 = Drum sound out1
   External pin 6 = Trigger
   External pin 7 = parameter

    ATTiny overview
                            +-\/-+
                     Reset 1|    |8  VCC
         (pin3) out2   PB3 2|    |7  PB2 parameter
         (pin4) out3   PB4 3|    |6  PB1 (pin1)  trigger
                       GND 4|    |5  PB0 (pin0)  sound
                            ------

   Description:
   Simple version of EHX space drum.  Ah...really...simple.
   Start and end frequency is random
   There is a "parameter" value too that will be used for "length",
   but feel free to change that.

   you can sum up for crazier sound

   V 1.0  -  First Version

   Rob Stave (Rob the Fiddler) CCBY 2020

*/

// Different sounds.  These are not samples, but algorithms.
//  Its just a start, feel free to mix/match and create your own.

// Debounce time
#define DEBOUNCE 30

volatile unsigned int Acc1; // main freq accumulator
volatile unsigned int Acc2; // main freq accumulator
volatile unsigned int Acc3; // main freq accumulator

volatile unsigned int StartNote = 800;   // tuning word start
volatile unsigned int EndNote = 800;     // tuning word end
volatile unsigned int ActualNote = 800;  // tuning word end
volatile unsigned int ActualNote2 = 800; // tuning word end
volatile unsigned int ActualNote3 = 800; // tuning word end

volatile unsigned int noteCounter = 0;

volatile unsigned int noteLengthMaxMillis = 500; // tuning word

volatile boolean isRunning = false;
volatile boolean setTrigger = false;

volatile unsigned int lfsr = 1;

// the setup function runs once when you press reset or power the board
void setup()
{

  DDRB = B00011001; // Set port B output bits

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

    Acc1 = Acc1 + ActualNote;
    Acc2 = Acc2 + ActualNote2;
    Acc3 = Acc3 + ActualNote3;

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

    // Write bit for sound
    uint8_t result2 = (Acc2 >> 8) & 0x80;
    if (result2 > 0)
    {
      bitSet(PORTB, 3);
    }
    else
    {
      bitClear(PORTB, 3);
    }
    // Write bit for sound
    uint8_t result3 = (Acc3 >> 8) & 0x80;
    if (result3 > 0)
    {
      bitSet(PORTB, 4);
    }
    else
    {
      bitClear(PORTB, 4);
    }
  }
  else
  {
    bitClear(PORTB, 0);
    bitClear(PORTB, 3);
    bitClear(PORTB, 4);
  }
}

int mapExp(int value, int timeStart, int timeEnd, int startFreq, int endFreq)
{

  int quarterTime = timeEnd >> 2;
  int middleFrequency = (startFreq + endFreq) >> 1;

  if (value < quarterTime)
  {
    return map(value, 0, quarterTime, startFreq, middleFrequency);
  }
  else
  {
    return map(value, quarterTime, timeEnd, middleFrequency, endFreq);
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

  // We will sample these values continuously.  Just so that we do not have to read all 3
  // when the trigger occurs.
  unsigned int startLive = 0;
  unsigned int endLive = 0;
  unsigned int lengthLive = 0;

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

    int reading = digitalRead(PB1);

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

      // int sample = analogRead(A3);
      int sample = lfsr >> 6;
      startLive = map(sample, 0, 1023, 3000, 300);

      // int sample2 = analogRead(A2);
      clockLfsr();
      clockLfsr();
      clockLfsr();
      clockLfsr();
      clockLfsr();
      int sample2 = lfsr >> 6;
      endLive = map(sample2, 0, 1023, 3000, 200);

      int sample3 = analogRead(A1);
      lengthLive = map(sample3, 0, 1023, 100, 900);

      sensorCounter = 0;
    }
    sensorCounter++;

    if (setTrigger == true)
    {

      // Kick off note.  Store off previous millis to track length
      isRunning = true;
      Acc1 = 0;
      setTrigger = false;
      noteCounter = 0;
      previousMillis = currentMillis;
      StartNote = startLive;
      EndNote = endLive;
      ActualNote = StartNote;

      noteLengthMaxMillis = lengthLive;
    }

    if (isRunning == true)
    {

      // The note will last as long as
      // noteCounter > noteLengthMaxMillis.  However, I have added below a "section"
      // as well.  Maybe a mini ASDR ?  We are just spliting the note in 6 sections.
      // so that we can do things like bitshift the accumulator step to get arp sounds.

      noteCounter = currentMillis - previousMillis;

      // The three notes are really determined here and some somewhat intertwined
      // The second note is the first one with some vibrato/freq modulation
      // the third note is a fifth  achieved by multiplying by 1.5 ( x + x>>1 is the same thing.)

      ActualNote = mapExp(noteCounter, 0, noteLengthMaxMillis, StartNote, EndNote);
      ActualNote2 = ActualNote - 64 + (noteCounter >> 6);
      ActualNote3 = (ActualNote >> 1) + ActualNote;

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
