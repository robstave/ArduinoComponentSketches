
/**
 * ACS-85-0505
 * ATTiny85  Nand Synth Generator
 *
 * Three nands. Similar to other nand oscillator ciruits
 *
 * Three knobs control two high frequencies and a low frequency that
 * can be toggled on and off
 *
 * Simulation of one of many nand oscillator noise circuits.
 *
 * External pin 1       = Reset (not used)
 * External pin 2 (PB3) = input 0 freq 1
 * External pin 3 (PB4) = input 1 freq 2
 * External pin 4       = GND
 * External pin 5 (PB0) = output 0 output
 * External pin 6 (PB1) = input switch LFO
 * External pin 7 (PB2) = input 2 freq 3 LFO
 * External pin 8       = Vcc
 *
 *
 * V 1.0  -  First Version
 *
 * Note: This sketch has been written specifically for ATTINY85 and not Arduino uno
 *
 * Rob Stave (Rob the fiddler) ccby 2015
 */



//  ATTiny overview
//                           +-\/-+
//                    Reset 1|    |8  VCC
//      (pin3) osc1 A3  PB3 2|    |7  PB2 (pin2) LFO Speed
//      (pin4) osc2 A2  PB4 3|    |6  PB1 (pin1) LFO on/off
//                      GND 4|    |5  PB0 (pin0) out 0
//                           ------

//Ranges for the pot.  Technically a small number means a
//shorter timer so low or high...whatever you want to call it.
#define VCO1HIGH 7
#define VCO1LOW 108

#define VCO2HIGH 7
#define VCO2LOW 108

#define VCO3HIGH 10
#define VCO3LOW 200

#define MAX_STATES 5

#define SELECT_PIN 1


//counters for the frequencies
//Using bytes as they are faster than ints.
//so do not use numbers higher that 255 without a double loop (see the LFO)
volatile byte oscFreq1 = 0;
volatile byte oscCounter1 = 0;

volatile byte oscFreq2 = 0;
volatile byte oscCounter2 = 0;

volatile byte lfoCount3 = 0;
volatile byte oscFreq3 = 0;
volatile byte oscCounter3 = 0;

//Switch lfo on/off
volatile boolean s0 = LOW;

volatile boolean osc1 = LOW;
volatile boolean osc2 = LOW;
volatile boolean osc3 = LOW;


//Initial state
byte state = 0;
//State used to do debounce
boolean debounceState = LOW;



int loopCount = 0;

// the setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00000001;

  pinMode(0, OUTPUT);
  pinMode(1, INPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  // initialize timer1
  noInterrupts();           // disable all interrupts

  TCCR1 = 0;                  //stop the timer
  TCNT1 = 0;                  //zero the timer
  //GTCCR = _BV(PSR1);          //reset the prescaler
  OCR1A = 100;                //set the compare value
  OCR1C = 100;
  TIMSK = _BV(OCIE1A);        //interrupt on Compare Match A

  TCCR1 = _BV(CTC1) | _BV(CS11); // Start timer, ctc mode, prescaler clk/2

  interrupts();             // enable all interrupts

}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{

  //Count up and toggle portB bits
  if (oscCounter1 > oscFreq1) {
    oscCounter1 = 0;
    osc1 = !osc1;
  }
  oscCounter1++;


  if (oscCounter2 > oscFreq2) {
    oscCounter2 = 0;
    osc2 = !osc2;
  }
  oscCounter2++;

  //LFO
  if (lfoCount3 > 33) {
    lfoCount3 = 0;
    if (oscCounter3 > oscFreq3) {
      oscCounter3 = 0;
      osc3 = !osc3;
    }
    oscCounter3++;
  }
  lfoCount3++;

  boolean result = LOW;

  //Simulate the circuits.

  if (state == 0) {
    result = !( (!(osc1 & osc3)) & osc2);
  } else if (state == 1) {
    result = !(osc2 & osc1);
  } else if (state == 2) {
    result = !( (!(osc1 & osc2)) & osc3);
  } else if (state == 3) {
    result = ( (osc1 ^ osc2) & osc3);
  }  else if (state == 4) {
    result = ( !(osc1 & osc3) & !(osc2 & !osc3));
  }  
  //above init assures that a bad state is just LOW

  if (result) {
    PORTB |=  (1 << PB0);           // Set port HIGH
  } else {
    PORTB &= ~(1 << PB0);          // Set port LOW
  }

}


void incr () {

  state++;

  if (state >= MAX_STATES) {
    state = 0;
  }


}


void loop() {

 
  //Read pots/inputs
  int osc1_t = analogRead(A3);
  oscFreq1 = map(osc1_t, 0, 1023,  VCO1HIGH, VCO1LOW);

  osc1_t = analogRead(A2);
  oscFreq2 = map(osc1_t, 0, 1023,  VCO2HIGH, VCO2LOW);

  osc1_t = analogRead(A1);
  oscFreq3 = map(osc1_t, 0, 1023,  VCO3HIGH, VCO3LOW);
 
 

  boolean selectPinRead = digitalRead(SELECT_PIN) ;
  if (selectPinRead == HIGH) {
    if (debounceState == LOW) {
      incr ();
      debounceState = HIGH;
    }

  } else {
    debounceState = LOW;
  }
}
