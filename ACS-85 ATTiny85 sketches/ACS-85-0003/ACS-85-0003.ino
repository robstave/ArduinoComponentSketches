
/**
 * ACS-85-0003
 * ATTiny85  Alternating frequencies vco
 * 
 * Two tone sound and sample hold sound
 * 3 inputs and 2 outputs.
 *
 * Pin 5 does a two tone sound based on two frequncies and a speed.
 * Pin 6 is a counter mash up of the other things.  Its kinda like
 * a sample hold.
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
 * V 1.2  -  Changed up variables a bit
 *
 * Note: This sketch has been written specifically for ATTINY85 and not Arduino uno
 *
 * Rob Stave (Rob the fiddler) ccby 2015, 2018
 */

//  ATTiny overview
//                      +-\/-+
//               Reset 1|    |8  VCC
// (pin3) in 0 A3  PB3 2|    |7  PB2 (pin2) A1 Speed
// (pin4) in 1 A2  PB4 3|    |6  PB1 (pin1) out 1
//                 GND 4|    |5  PB0 (pin0) out 0
//                      ------

//Ranges for the pot.  Technically a small nuumber means a
//shorter timer so low or high...whatever you want to call it.

#define VCO1HIGH 7  //low number for highest freq
#define VCO1LOW 108 //bigger number for lower freq (byte)

#define VCO2HIGH 7 //low number for highest freq
#define VCO2LOW 108 //bigger number for lower freq (byte)

#define VCO3HIGH 1200  //LFO counters
#define VCO3LOW 7000


//counters for the frequencies
volatile byte oscFreq1 = VCO1LOW;
volatile byte oscCounter1 = VCO1LOW;
volatile byte oscFreq2 = VCO2LOW;
volatile byte oscCounter2 = VCO2LOW;
volatile uint16_t oscFreq3 = VCO3LOW;
volatile uint16_t oscCounter3 = VCO3LOW;

volatile uint16_t oscFreq4 = VCO3HIGH;
volatile uint16_t oscCounter4 = VCO3HIGH;

volatile boolean oscState = false;


// the setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00000011;  //set PB0, PB1 as outputs

  // initialize timer1
  noInterrupts();           // disable all interrupts

  TCCR1 = 0;                  //stop the timer
  TCNT1 = 0;                  //zero the timer
  GTCCR = _BV(PSR1);          //reset the prescaler
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
    if (oscCounter1 <= 0) {
      oscCounter1 = oscFreq1;
      PORTB ^= (_BV(PB0));
    }
    oscCounter1--;
    
  } else {

    if (oscCounter2 <= 0) {
      oscCounter2 = oscFreq2;
      PORTB ^= (_BV(PB0));
    }
    oscCounter2--;
  }

  // also, each period based on osc freq, calc a freq based on other values. Maybe like a mod.
  if (oscCounter3 <= 0) {
    oscCounter3 = oscFreq3;
    oscState = !oscState;

    //Sample abs difference to get weird frequency
    oscFreq4 = abs(oscCounter2 - oscCounter1);
  }
  oscCounter3--;

  //Beep the weird frequency
  if (oscCounter4 <= 0) {
    oscCounter4 = oscFreq4;
    PORTB ^= (_BV(PB1));
  }
  oscCounter4--;
  
}

void loop() {
  uint16_t osc1_t = analogRead(A3);
  oscFreq1 = map(osc1_t, 0, 1023,  VCO1HIGH, VCO1LOW);
  osc1_t = analogRead(A2);
  oscFreq2 = map(osc1_t, 0, 1023,  VCO2HIGH, VCO2LOW);
  osc1_t = analogRead(A1);
  oscFreq3 = map(osc1_t, 0, 1023,  VCO3HIGH, VCO3LOW);
}
