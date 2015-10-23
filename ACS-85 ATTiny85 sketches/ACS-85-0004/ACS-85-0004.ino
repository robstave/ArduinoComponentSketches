
/**
 * ACS-85-0004
 * ATTiny85  Dual VCO Selectable Range
 *
 * Dual frequncy vco
 * With an analog control to do ranges from LFO to High frequency
 *
 * 6 ranges HF1,HF2, MF1, MF2, LF1, LF2
 *
 * External pin 1       = Reset (not used)
 * External pin 2 (PB3) = input 0 freq 1
 * External pin 3 (PB4) = input 1 freq 2
 * External pin 4       = GND
 * External pin 5 (PB0) = output 0 output
 * External pin 6 (PB1) = Output 1 output
 * External pin 7 (PB2) = input 2 Select Range
 * External pin 8       = Vcc
 *
 * V 1.0  -  First Version
 *
 * Note: This sketch has been written specifically for ATTINY85 and not Arduino uno
 * Observations.
 *
 * I could not reach 4k freq with a long counter. I think the interrupt code was too
 * much.  There might be a few things I can do to tweak this.   Instead, I just used a scalar bool
 * isLFO and really copied the code.  Yes, copied.  There is plenty of room on the chip for this
 * code, so just unwind the code and duplicate it rather than add clock cycles to make it
 * look tighter.
 *
 * so if you add code to your interrupt, get out a frequncy counter and make sure
 * that its hitting what you think it is.  If not, your interrupt is attempting to interrupt your interrupt.
 * since it can not do that..it just gives you a lower frequncy.
 *
 * If you absolutly have to have above 4k, look into using the timers directly to pwm
 *
 * Rob Stave (Rob the fiddler) ccby 2015
 */



//  ATTiny overview
//                           +-\/-+
//                    Reset 1|    |8  VCC
//      (pin3) in 0 A3  PB3 2|    |7  PB2 (pin2) A1 Range
//      (pin4) in 1 A2  PB4 3|    |6  PB1 (pin1) out 1
//                      GND 4|    |5  PB0 (pin0) out 0
//                           ------

//Ranges for the pot.  Technically a small nuumber means a
//shorter timer so low or high...whatever you want to call it.

/**
 * For the HF and Medium Frequncies, we set the OCR1A = 100
 * So to figure out the frequencies, Divide you divisor by the number*2
 *
 * 8mhz/2/(99+1) = 40000
 * divide by number*2 (because its a squarewave)
 */

// 6 -> 4khz
// 80 -> 40000/80/2 =  250hz

#define VCO1_H1_HIGH 5
#define VCO1_H1_LOW 80
#define VCO2_H1_HIGH 5
#define VCO2_H1_LOW 80


// 30 -> 666 hz
// 120 - > 166 hz
#define VCO1_H2_HIGH 30
#define VCO1_H2_LOW 120
#define VCO2_H2_HIGH 30
#define VCO2_H2_LOW 120

// 90 -> 222hz
// 320 -> 62.5
#define VCO1_M1_HIGH 90
#define VCO1_M1_LOW 320
#define VCO2_M1_HIGH 90
#define VCO2_M1_LOW 320

// 150 -> 133
// 520 - > 38.5
#define VCO1_M2_HIGH 150
#define VCO1_M2_LOW 520
#define VCO2_M2_HIGH 150
#define VCO2_M2_LOW 520


/**
 * Note in the switch, we change it up with an extra
 * nested loop of 50
 *
 * so
 * 8mhz/2/100/50 = 800hz Interrupt freq
 */

//25 -> 800/2 = 16
// 400  -> 1
#define VCO1_L1_HIGH 25
#define VCO1_L1_LOW 400
#define VCO2_L1_HIGH 25
#define VCO2_L1_LOW 400


//100 -> 4
//2000 -> .2
#define VCO1_L2_HIGH 100
#define VCO1_L2_LOW 2000
#define VCO2_L2_HIGH 100
#define VCO2_L2_LOW 2000



//counters for the frequencies
int oscFreq1 = 0;
int oscCounter1 = 0;
int oscFreq2 = 0;
int oscCounter2 = 0;

boolean isLFO = false;
int lfoCounter = 0;

int range = 0;  //from 0 to 5
int loopCount = 0;


// the setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00000011;  //set output bits

  // initialize timer1
  noInterrupts();           // disable all interrupts

  TCCR1 = 0;                  //stop the timer
  TCNT1 = 0;                  //zero the timer
  //GTCCR = _BV(PSR1);          //reset the prescaler
  OCR1A = 99;                //set the compare value
  OCR1C = 99;
  TIMSK = _BV(OCIE1A);        //interrupt on Compare Match A

  TCCR1 = _BV(CTC1) | _BV(CS11); // Start timer, ctc mode, prescaler clk/2

  interrupts();             // enable all interrupts

}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{

  if (isLFO == true) {
    //Count up and toggle portB bits
    if (lfoCounter > 50) {
      lfoCounter = 0;

      //Count up and toggle portB bits
      if (oscCounter1 > oscFreq1) {
        oscCounter1 = 0;
        PORTB ^= (_BV(PB0));
      }
      oscCounter1++;

      if (oscCounter2 > oscFreq2) {
        oscCounter2 = 0;
        PORTB ^= (_BV(PB1));
      }
      oscCounter2++;

    }
    lfoCounter++;

    return;
  }

  //Count up and toggle portB bits
  if (oscCounter1 == 0) {
    oscCounter1 = oscFreq1;
    PORTB ^= (_BV(PB0));
  }
  oscCounter1--;

  if (oscCounter2 == 0) {
    oscCounter2 = oscFreq2;
    PORTB ^= (_BV(PB1));
  }
  oscCounter2--;

}

void loop() {

  //no need to monitor range that often
  if (loopCount > 20) {
    loopCount = 0;
    int   range_t = analogRead(A1);
    range = map(range_t, 0, 1023, 0,  60);
  }
  loopCount++;


  int osc1_t = analogRead(A3);
  int osc2_t = analogRead(A2);

  if (range < 10) {

    isLFO = true;
    oscFreq1 = map(osc1_t, 0, 1023, VCO1_L2_LOW,  VCO1_L2_HIGH);
    oscFreq2 = map(osc2_t, 0, 1023, VCO2_L2_LOW,  VCO2_L2_HIGH);
    return;
  }

  if (range < 20) {
    isLFO = true;
    oscFreq1 = map(osc1_t, 0, 1023, VCO1_L1_LOW,  VCO1_L1_HIGH);
    oscFreq2 = map(osc2_t, 0, 1023, VCO2_L1_LOW,  VCO2_L1_HIGH);
    return;
  }

  if (range < 30) {
    isLFO = false;
    oscFreq1 = map(osc1_t, 0, 1023, VCO1_M2_LOW,  VCO1_M2_HIGH);
    oscFreq2 = map(osc2_t, 0, 1023, VCO2_M2_LOW,  VCO2_M2_HIGH);
    return;

  }

  if (range < 40) {
    isLFO = false;
    oscFreq1 = map(osc1_t, 0, 1023, VCO1_M1_LOW,  VCO1_M1_HIGH);
    oscFreq2 = map(osc2_t, 0, 1023, VCO2_M1_LOW,  VCO2_M1_HIGH);
    return;
  }

  if (range < 50) {
    isLFO = false;
    oscFreq1 = map(osc1_t, 0, 1023, VCO1_H2_LOW,  VCO1_H2_HIGH);
    oscFreq2 = map(osc2_t, 0, 1023, VCO2_H2_LOW,  VCO2_H2_HIGH);
    return;
  }

  isLFO = false;
  oscFreq1 = map(osc1_t, 0, 1023, VCO1_H1_LOW,  VCO1_H1_HIGH);
  oscFreq2 = map(osc2_t, 0, 1023, VCO2_H1_LOW,  VCO2_H1_HIGH);
  return;


}
