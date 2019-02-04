
/**
   ACS-85-0540
   ATTiny85  4 Channel VCO with LFO (on/off)

   4 pins output a squarewave tone that is controlled by an analog input.
   Pins are f, f/2, f/3, f/6.
   The Tone randomly turns on and off.

   You can mix the pins with ANDs or resistors.

   LFSR is used to turn the pin on and off.

   There is a Warble effect that can be turned on/off programatically.

   External pin 1 = Reset
   External pin 2 = Freq/6
   External pin 3 = Freq/3
   External pin 4 = GND
   External pin 5 = Freq/2
   External pin 6 = Freq
   External pin 7 = Analog input freq
   External pin 8 = VCC

   V 1.0  -  First Version

   Rob Stave (Rob the fiddler) 2019 CCBY
*/



//  ATTiny overview
//                               +-\/-+
//                       Reset  1|    |8  VCC
//          Freq/3 (pin3) PB3   2|    |7   PB2 (pin2) Frequency Adjust (A1)
//          Freq/6 (pin4) PB4   3|    |6   PB1 (pin1) Freq / 2
//                         GND  4|    |5   PB0 (pin0) Freq


// set to zero for no warble ( 5 is good)
#define WARBLE 0
#define VCO_LO 80   //A bigger number means a longer count = lower freq
#define VCO_HIGH 8


#define VAR1 A1         //Analog read pin


// LFSR - Used to randomly turn pin on/off.  Each channel is a different speed, so
// separate ones are needed
volatile uint16_t lfsr1 = 1;
volatile uint16_t lfsr2 = 2;
volatile uint16_t lfsr3 = 3;
volatile uint16_t lfsr0 = 4;

// counters for the frequencies in ISR
volatile uint16_t mycount = 1;
volatile uint8_t mycount3 = 1;
volatile uint16_t  oscFreq = 55; //I just picked a number, it gets written over real quick
volatile uint16_t  oscCounter = 0;


void setup() {
  // 4 outs, 1 input
  DDRB = B00011011;

  noInterrupts();           // disable all interrupts

  // initialize timers
  //  Using one for a tone and the other for a millis timer

  TCCR0A = 0;
  TCCR0B = 0;

  TCCR0A |= (1 << WGM01); //Start timer 1 in CTC mode Table 11.5

  OCR0A = 39; //CTC Compare value

  TCCR0B |= (1 << CS01); // Prescaler =0 Table 11.6

  // TCCR0A |=(1<<COM0A1); //Timer0 in toggle mode Table 11.2
  TIMSK |= (1 << OCIE0A); //Enable CTC interrupt see 13.3.6

  // start the timer, prescaler
  TCCR1 = (1 << CTC1) | (7 << CS10); // CTC  mode, div64
  //OCR1C = 0.001 * F_CPU / 64 - 1; // 1ms, F_CPU  , div64

  //  8Mhz/(64*(OCR0A +1)) = 8mhz/(64*(124+1)) = 1000hz
  OCR1C = 124;
  TIMSK |= (1 << OCIE1A);

  interrupts();             // enable all interrupts
}

// Millis Timer
volatile unsigned long milliseconds;

ISR(TIMER1_COMPA_vect)
{
  milliseconds++;
}
unsigned long millis1()
{
  return milliseconds;
}


// Used to generate the sound.  We are just looping through some
// counters and in the main loop, setting the pin bits on that
// value, so we are dealing with oactaves and 1/3 of octaves
ISR(TIMER0_COMPA_vect)          // timer compare interrupt service routine
{
  if (oscCounter > oscFreq) {
    oscCounter = 0;
    mycount++;
    // div by 3 count too
    if ( (mycount % 3) == 0) {
      mycount3++;
    }

    byte reg0 = mycount  & lfsr0 & B00000001;
    byte reg1 = mycount  & lfsr1 & B00000010;
    byte reg2 = mycount3 & lfsr2 & B00000001;
    byte reg3 = mycount3 & lfsr3 & B00000010;

    if (reg0 > 0) {
      PORTB |=  (1 << PB0);           // Set port High
    } else {
      PORTB &= ~(1 << PB0);
    }
    if (reg1 > 0) {
      PORTB |=  (1 << PB1);           // Set port High
    } else {
      PORTB &= ~(1 << PB1);
    }

    if (reg2 > 0) {
      PORTB |=  (1 << PB3);           // Set port High
    } else {
      PORTB &= ~(1 << PB3);
    }

    if (reg3 > 0) {
      PORTB |=  (1 << PB4);           // Set port High
    } else {
      PORTB &= ~(1 << PB4);
    }
  }
  oscCounter++;
}


// Clock a 16 bit value as a lfsr
uint16_t clockLfsr (uint16_t lfsr) {

  //calculate new state
  boolean outputBit = bitRead(lfsr, 10) ^ bitRead(lfsr, 12)
                      ^ bitRead(lfsr, 13) ^ bitRead(lfsr, 15);
  lfsr = lfsr << 1;
  lfsr |= outputBit;
  return lfsr;
}

typedef struct PinState {
  int delay;
  unsigned long ts;
};

// get number between 150 and 220
int getRandomDelay() {
  return random(0, 70) + 150;
}


void loop() {


  unsigned long prevSample1 =  millis1();

  // init states
  PinState pin0 =   {getRandomDelay(), prevSample1};
  PinState pin1 =   {getRandomDelay(), prevSample1};
  PinState pin2 =   {getRandomDelay(), prevSample1};
  PinState pin3 =   {getRandomDelay(), prevSample1};

  unsigned int warble_count = 0;
  while (true) {

    unsigned long millsSample = millis1();

    //Sample every x ms to get new data
    // lets do it 12.5 times a second
    if ((millsSample - prevSample1) > 80 ) {

      warble_count++;

      prevSample1 = millsSample;
      int osc1_t = analogRead(VAR1);
      int sample = map(osc1_t, 0, 1023, VCO_HIGH,  VCO_LO);

      if (WARBLE > 0){
        oscFreq = sample + warble_count % WARBLE;
      } else {
        oscFreq = sample;
      }
      
    }

    //Clock LFSR for LSB to mask pin state
    // This is the LFO on/off for each pin

    if ((millsSample - pin0.ts) > pin0.delay ) {
      pin0.ts = millsSample;
      lfsr0 = clockLfsr(lfsr0);
      pin0.delay  = getRandomDelay();
    }

    if ((millsSample - pin1.ts) > pin1.delay ) {
      pin1.ts = millsSample;
      lfsr1 = clockLfsr(lfsr1);
      pin1.delay  = getRandomDelay();
    }

    if ((millsSample - pin2.ts) > pin2.delay ) {
      pin2.ts = millsSample;
      pin2.delay  = getRandomDelay();
      lfsr2 = clockLfsr(lfsr2);
    }

    if ((millsSample - pin3.ts) > pin3.delay ) {
      pin3.ts = millsSample;
      pin3.delay  = getRandomDelay();
      lfsr3 = clockLfsr(lfsr3);
    }

  }

}


