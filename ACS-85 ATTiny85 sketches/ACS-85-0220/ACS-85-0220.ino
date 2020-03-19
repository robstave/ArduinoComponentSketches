/**
   ACS-85-0220 ATTiny85 - debounced Trigger
   
   Basically a trigger to gate, but it can subdivide the gates.
   The length of the total gate time remains the same, but adding ticks
   subdivides it

   The intent was to have a trigger that was like a half note, but you could
   trigger 1/16 notes on the fly over the same gate.

   External pin 2 = Number of ticks ( analog represneting 1 to 8 ticks)
   External pin 3 = Blip Length
   External pin 5 = Gate out
   External pin 6 = none
   External pin 7 = Trigger

    ATTiny overview
                            +-\/-+
                     Reset 1|    |8  VCC
         (pin3) ticks  PB3 2|    |7  PB2 trigger
         (pin4) length PB4 3|    |6  PB1 (pin1)  none
                       GND 4|    |5  PB0 (pin0)  output (gate)
                            ------

   Description:
   Debounced gate
   Length is the length of the total gate
   Ticks > 1 will subdivide the gate.

   so for example...a value of 6 actually is gives and output of

   on - off - on - off - on - off  over the course of the length.

   Trigger:          ---xx------------------
   Length:           ---xxxxxxxxxxxx--------
   Gate(ticks = 1)   ---xxxxxxxxxxxx--------
   Gate(ticks = 2)   ---xxxxxx--------------
   Gate(ticks = 3)   ---xxxx----xxxx--------
   Gate(ticks = 4)   ---xxx---xxx-----------
   Gate(ticks = 6)   ---xx--xx--xx------------

   its not super accurate...but gives an interesting effect when
   used lightly.  

   V 1.0  -  First Version
   Rob Stave (Rob the Fiddler) CCBY 2019
*/

// Debounce time
# define DEBOUNCE 30

# define MAX_LENGTH 500
# define MIN_LENGTH 40
volatile unsigned int noteCounter = 0;
volatile unsigned int noteLengthMillis = 500;
 volatile unsigned int ticks = 4;

volatile boolean isRunning = false;
volatile boolean setTrigger = false;

// the setup function runs once when you press reset or power the board
void setup()
{

  DDRB = B00000011; //Set port B output bits

  // initialize timer
  noInterrupts(); // disable all interrupts

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


void loop()
{

  unsigned long previousMillis = 0;


  // no need to check speed all that often
  // so we space that out a bit
  byte sensorCounter = 0;
  byte sensorLimit = 500;
  byte checkState = 0;

  int buttonState;             // the current reading from the input pin
  int lastButtonState = LOW;   // the previous reading from the input pin
  unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
  unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
  while (true) {


    // Debound stuff
    unsigned long currentMillis = millisTimer1();

    int reading = digitalRead(PB2);

    // check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH), and you've waited long enough
    // since the last press to ignore any noise:

    // If the switch changed, due to noise or pressing:
    if (reading != lastButtonState) {
      // reset the debouncing timer
      lastDebounceTime = currentMillis;
    }

    if ((currentMillis - lastDebounceTime) > debounceDelay) {
      // whatever the reading is at, it's been there for longer than the debounce
      // delay, so take it as the actual current state:

      // if the button state has changed:
      if (reading != buttonState) {
        buttonState = reading;

        // only toggle the trigger if the new button state is HIGH
        if (buttonState == HIGH) {
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
    if (sensorCounter > sensorLimit) {
      if (checkState % 2 == 0) {
        int sample = analogRead(A3);
        ticks = map(sample, 0, 1023, 1, 8);
      }  else {
        int sample3 = analogRead(A2);
        noteLengthMillis = map(sample3, 0, 1023, MIN_LENGTH, MAX_LENGTH);
      }

      sensorCounter = 0;
      checkState++;
    }
    sensorCounter++;

    if  (setTrigger == true) {
      isRunning = true;
      setTrigger = false;
      noteCounter = 0;
      previousMillis = currentMillis;
    }

    if (isRunning == true) {
      noteCounter = currentMillis - previousMillis;
      if (noteCounter >= noteLengthMillis)
      {
        // Note is finished
        isRunning = false;
        setTrigger = false;
        previousMillis = currentMillis;
      }


      int multiplier = map( noteCounter, 0, noteLengthMillis, 1, ticks);

      if (isRunning == true && ( multiplier % 2 == 1)) {
        bitSet(PORTB, 0);
      }  else {
        bitClear(PORTB, 0);
      }


    } else {
      setTrigger = false;
      noteCounter = 0;
    }

  }
}
