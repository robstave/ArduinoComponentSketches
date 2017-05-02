
/**
 * ACS-85-0003
 * ATTiny85  Alternating frequencies vco
 * 
 * Two tone sound
 * 3 inputs and 2 outputs.
 *
 * Pin 0 does a two tone sound based on two frequncies and a speed.
 * Pin 1 is a counter mash up of the other things.  Its fun and wierd.
 * 
 * External pin 1       = Reset (not used)
 * External pin 2 (PB3) = input 0 freq 1
 * External pin 3 (PB4) = input 1 freq 2
 * External pin 4       = GND
 * External pin 5 (PB0) = output 0 output
 * External pin 6 (PB1) = output 1 weird output
 * External pin 7 (PB2) = input 2 Speed
 * External pin 8       = Vcc
 *
 *
 * V 1.0  -  First Version
 * V 1.1  -  comments
 *
 * Note: This sketch has been written specifically for ATTINY85 and not Arduino uno
 *
 * Rob Stave (Rob the fiddler) ccby 2015
 */



//  ATTiny overview
//                           +-\/-+
//                    Reset 1|    |8  VCC
//      (pin3) in 0 A3  PB3 2|    |7  PB2 (pin2) A1 Speed
//      (pin4) in 1 A2  PB4 3|    |6  PB1 (pin1) out 1
//                      GND 4|    |5  PB0 (pin0) out 0
//                           ------

//Ranges for the pot.  Technically a small nuumber means a
//shorter timer so low or high...whatever you want to call it.
#define VCO1HIGH 7
#define VCO1LOW 108

#define VCO2HIGH 7
#define VCO2LOW 108

#define VCO3HIGH 1200
#define VCO3LOW 7000


//counters for the frequencies
int oscFreq1 = 0;
int oscCounter1 = 0;
int oscFreq2 = 0;
int oscCounter2 = 0;
int oscFreq3 = 0;
int oscCounter3 = 0;

int oscFreq4 = 0;
int oscCounter4 = 0;

boolean oscState = false;


// the setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00000011;  

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
   
  //toggle alternating frequency
  // if osc state is true/false we calc the freq based 
  // on checking oscFreq1 or oscFreq2

  if (oscState == true) {
    //Count up and toggle portB bits
    if (oscCounter1 > oscFreq1) {
      oscCounter1 = 0;
      PORTB ^= (_BV(PB0));
    }
    oscCounter1++;
    
  } else {

    if (oscCounter2 > oscFreq2) {
      oscCounter2 = 0;
      PORTB ^= (_BV(PB0));
    }
    oscCounter2++;
  }

  // also, each period based on osc freq, calc a freq based on other values. Maybe like a mod.
  if (oscCounter3 > oscFreq3) {
    oscCounter3 = 0;
    oscState = !oscState;

    //Sample abs difference to get wierd frequency
    oscFreq4 = abs(oscCounter2 - oscCounter1);
  }
  oscCounter3++;

  //Beep the weird frequency
  if (oscCounter4 > oscFreq4) {
    oscCounter4 = 0;
    PORTB ^= (_BV(PB1));
  }
  oscCounter4++;
  
}

void loop() {

  int osc1_t = analogRead(A3);
  oscFreq1 = map(osc1_t, 0, 1023,  VCO1HIGH, VCO1LOW);
  osc1_t = analogRead(A2);
  oscFreq2 = map(osc1_t, 0, 1023,  VCO2HIGH, VCO2LOW);
  osc1_t = analogRead(A1);
  oscFreq3 = map(osc1_t, 0, 1023,  VCO3HIGH, VCO3LOW);

}
