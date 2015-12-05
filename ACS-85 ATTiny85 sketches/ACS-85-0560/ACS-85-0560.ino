
/**
 * ACS-85-0560
 * ATTiny85  Sampler
 *
 * Sample the value of a pin and replay it or erase it at different speeds
 *
 * External pin 1       = Reset (not used)
 * External pin 2 (PB3) = Speed
 * External pin 3 (PB4) = Value
 * External pin 4       = GND
 * External pin 5 (PB0) = output
 * External pin 6 (PB1) = write
 * External pin 7 (PB2) = erase
 * External pin 8       = Vcc
 *
 * Inspired by the Luna Mod Looper.
 * I suppose if somebody wants to use this as a starting pad to
 * get something closer, go for it.
 * http://makezine.com/projects/make-26/the-luna-mod-looper/
 *
 * Has two knobs.  One is the freq and the other is the speed that we go
 * through the buffer.  The two buttons are for write and clear.
 * To Clear the whole buffer, just press it for a long time.
 *
 *
 *
 * V 1.0  -  First Version
 *
 * Note: This sketch has been written specifically for ATTINY85 and not Arduino uno
 *
 * Rob Stave (Rob the fiddler) CCBY 2015
 */



//  ATTiny overview
//                           +-\/-+
//                    Reset 1|    |8  VCC
//      (pin3) in 0 A3  PB3 2|    |7  PB2 (pin2) erase
//      (pin4) in 1 A2  PB4 3|    |6  PB1 (pin1) write
//                      GND 4|    |5  PB0 (pin0) out 0
//                           ------

//Ranges for the pot.  Technically a small nuumber means a
//shorter timer so low or high...whatever you want to call it.


#define VCOHIGH 7
#define VCOLOW 188


//counters for the frequencies
int oscFreq1 = 0;
int readFreq1 = 0;

int sampleLength = 1;
int sampleCount = 0;

boolean doWrite = LOW;
boolean doClear = LOW;

int oldRead = 0;
int newRead = 0;


byte oscCounter1 = 0;

byte buttonCounter = 0;
byte lengthCounter = 0;

byte patternCount = 0;
#define BUFFER_SIZE 256
byte  sampleBuffer[BUFFER_SIZE] = {
  0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

  0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

  0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

  0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


// the setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00000001; // set PORTB to inputs except PB0

  // initialize timer1
  noInterrupts();           // disable all interrupts

  TCCR1 = 0;                  //stop the timer
  TCNT1 = 0;                  //zero the timer
  //GTCCR = _BV(PSR1);          //reset the prescaler
  OCR1A = 66;                //set the compare value
  OCR1C = 66;
  TIMSK = _BV(OCIE1A);        //interrupt on Compare Match A

  TCCR1 = _BV(CTC1) | _BV(CS11); // Start timer, ctc mode, prescaler clk/2

  interrupts();             // enable all interrupts

  oscFreq1 = sampleBuffer[0];

}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{

  if (oscFreq1 > 0) {
    //Count up and toggle portB bits
    if (oscCounter1 > oscFreq1) {
      oscCounter1 = 0;
      PORTB ^= (_BV(PB0));
    }
    oscCounter1++;
  }

  if (sampleCount >= sampleLength) {
    sampleCount = 0;

    //Write
    if (doWrite == HIGH) {
      sampleBuffer[ patternCount] = readFreq1;
    }

    //clear
    if (doClear == HIGH) {
      sampleBuffer[ patternCount] = 0;
    }

    // Fetch the value from the buffer
    oscFreq1 = sampleBuffer[ patternCount];

    patternCount++;
    if (patternCount >= 256) {
      patternCount = 0;
    }
  }
  sampleCount++;
}



/**
 * We prioritize the reading of values.
 * The most important is the freq.  Buttons can skip a few cycles
 * and the length is not really super important.
 *
 */
void loop() {

  if (lengthCounter > 6) {
    lengthCounter = 0;
    int len_t = analogRead(A3);
    sampleLength = map(len_t, 0, 1023, 100 ,  1000);
  }
  lengthCounter++;

  int read_t = analogRead(A2);

  oldRead = readFreq1;
  newRead = map(read_t, 0, 1023, VCOHIGH,  VCOLOW);

  readFreq1 = (oldRead +  newRead) >> 1;




  if (buttonCounter > 2) {
    buttonCounter = 0;
    byte inputPortB = PINB;
    doWrite = bitRead(inputPortB, 1);
    doClear = bitRead(inputPortB, 2);
  }

  buttonCounter++;

}
