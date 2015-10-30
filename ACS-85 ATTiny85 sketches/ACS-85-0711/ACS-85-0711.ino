

/**
 * ACS-85-0711 ATTiny85  Button select. 7 state
 *
 *  Control sketch that monitors two inputs
 *  to increment/decrement a count.
 *
 *  There is a software switch as well that allows the
 *  state to wrap
 *
 * External pin 2 = up
 * External pin 3 = down
 * External pin 5 = Bit2
 * External pin 6 = Bit1
 * External pin 7 = Bit0
 *
 * Rob Stave (Rob the Fiddler) CCBY 2015
 *
  */
//  ATTiny overview
//                           +-\/-+
//                   Reset  1|    |8  VCC
//                 up  PB4  2|    |7  PB2 (pin2) Bit2
//               down  PB3  3|    |6  PB1 (pin1) Bit1
//                     GND  4|    |5  PB0 (pin0) Bit0


/******
 * 
 * LOOP_DELAY
 * 
 * Effectively, the debounce delay as well.
 * If you have flicker problems, increase this.
 * If its too long, you will have a latency.
 * If you need a number more than 100, get a new switch
 **/
#define LOOP_DELAY 30


/*******
 * doWrap
 * Boolean that sets behavior of the state when
 * the limit is reached
 * Low = no wrap.  bottom and top has a ceiling
 * High = wrap.  State loops
 *
 */
boolean doWrap = LOW;

//Initial state  wil be a value [0,7]
int state = 0;
//State used to do debounce
boolean debounceUp = LOW;
boolean debounceDown = LOW;


#define UP_PIN 3
#define DOWN_PIN 4

int prevSample1; //timer value


// the setup function runs once when you press reset or power the board
void setup() {
  DDRB = B00000111;
}

void incr () {

  state++;
  if (doWrap == HIGH ) {
    if (state >= 8) {
      state = 0;
    }
  } else {
    if (state >= 8) {
      state = 7;
    }
  }
}

void decr () {

  state--;
  if (doWrap == HIGH ) {
    if (state < 0) {
      state = 7;
    }
  } else {
    if (state < 0) {
      state = 0;
    }
  }

}

void loop() {
  int millsSample = millis();

  //Sample every x ms
  if ((millsSample - prevSample1) > LOOP_DELAY ) {
    prevSample1 = millsSample;

    boolean downPinRead = digitalRead(DOWN_PIN) ;
    boolean upPinRead = digitalRead(UP_PIN) ;

    //Debounce is done because it takes two passes through
    //this part to call incr/decr
    if (upPinRead==HIGH) {
      if (debounceUp == LOW) {
        incr ();
        debounceUp = HIGH;
      }
    } else {
      debounceUp = LOW;
    }

    if (downPinRead ==HIGH) {
      if (debounceDown == LOW) {
        decr ();
        debounceDown = HIGH;
      }
    }else {
      debounceDown = LOW;
    }

    //Output the lower 3 bits as a 3 bit number
    byte output = B00000111 & state;
    PORTB = output;
  }

}
