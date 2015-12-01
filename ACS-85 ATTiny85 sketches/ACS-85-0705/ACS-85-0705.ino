
/**
 * ACS-85-0705
 * ATTiny85 Two Tap LFO
 * 
 * Tap the button and the output will reflect the speed with a 50% duty cycle
 *
 * Trigger is just there for debugging
 *
 * Notes:
 * For the longest time, I could never get my timer code to work
 * with millis.  Millis uses timer 0, so I should not have a problem
 * with timer 1.  I found the culprit to be this
 *
 *  I used to use this
 *  TIMSK = _BV(OCIE1A);        //interrupt on Compare Match A
 *
 *  but that blasts things.  This works better
 *  TIMSK |= (1<<OCIE1A);

 * Rob Stave (Rob the fiddler) ccby 2015
 */

//  ATTiny overview
//                           +-\/-+
//                    Reset 1|    |8  VCC
//      (pin3) Tap1     PB3 2|    |7  PB2 (pin2) trigger
//      (pin4) Tap2     PB4 3|    |6  PB1 (pin1) out Tap2
//                      GND 4|    |5  PB0 (pin0) out Tap1
//                           ------





#define LFO_HIGH 25l
#define LFO_LOW 400l

#define TAP1_PIN 3
#define TAP2_PIN 4

#define LOOP_DELAY 20

//State used to do debounce
boolean debounceTap1 = LOW;
boolean debounceTap2 = LOW;


//counters for the frequencies
int oscFreq1 = 225;
int oscCounter1 = 0;
int oscFreq2 = 225;
int oscCounter2 = 0;

unsigned long count = 0;
unsigned long count1snap = 0;
unsigned long count2snap = 0;

int prevSample1 = 0;

// the setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00000111;  //set output bits

  // initialize timer1
  noInterrupts();           // disable all interrupts

  TCCR1 = 0;                  //stop the timer
  TCNT1 = 0;                  //zero the timer
  //GTCCR = _BV(PSR1);          //reset the prescaler
  OCR1A = 99;                //set the compare value
  OCR1C = 99;

  TIMSK |= (1 << OCIE1A); //interrupt on Compare Match A  /works with timer

  TCCR1 = _BV(CTC1) | _BV(CS11)  | _BV(CS13); // Start timer, ctc mode, prescaler clk/2

  interrupts();             // enable all interrupts
}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{

  count++;

  //Count up and toggle portB bits
  if (oscCounter1 >= oscFreq1) {
    oscCounter1 = 0;
    PORTB ^= (_BV(PB0));
  }
  oscCounter1++;

  if (oscCounter2 >= oscFreq2) {
    oscCounter2 = 0;
    PORTB ^= (_BV(PB1));
  }
  oscCounter2++;

  return;
}

void loop() {

  int millsSample = millis();

  //Sample every 10ms
  if ((millsSample - prevSample1) > LOOP_DELAY ) {
    prevSample1 = millsSample;

   

    boolean tap1PinRead = digitalRead(TAP1_PIN) ;

    if (tap1PinRead == HIGH) {
      

      if (debounceTap1 == LOW) {

        unsigned long diff =  count - count1snap;

        if ( diff < LFO_LOW) {
          int newCount = (int)diff >> 1;

          //if close, average
          if  (abs(newCount - oscFreq1) < 100) {
            oscFreq1 = (oscFreq1 + newCount) >> 1;
          } else {
            oscFreq1 = newCount;
          }

          oscCounter1 = 0;
          PORTB |= B00000001;
        }
        count1snap = count;
        debounceTap1 = HIGH;
      }
    } else {
      debounceTap1 = LOW;
      
    }

    boolean tap2PinRead = digitalRead(TAP2_PIN) ;
    
    if (tap2PinRead == HIGH) {
      
      if (debounceTap2 == LOW) {
        unsigned long diff = count - count2snap;

        if ( diff < LFO_LOW) {
          int newCount = (int)diff >> 1;

          //if close, average
          if  (abs(newCount - oscFreq2) < 100) {
            oscFreq2 = (oscFreq2 + newCount) >> 1;
          } else {
            oscFreq2 = newCount;
          }
          
          oscCounter2 = 0;
          PORTB |= B00000010;
        }
        count2snap = count;
        debounceTap2 = HIGH;
      }
    } else {
      debounceTap2 = LOW;
    }

    if (tap1PinRead||tap2PinRead){
      PORTB |= B00000100;
    } else {
      PORTB &= B11111011;
    }

  }

}


 
