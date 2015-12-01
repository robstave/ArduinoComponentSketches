

/**
 * ACS-85-0710 ATTiny85  Button select.
 *
 *  Control sketch that monitors two inputs
 *  to toggle individual pins.
 *
 *  The inital state is {1,0,0}
 *  setting the up switches the state to {0,1,0} and {0,0,1}
 *
 *  u d  state
 *  0 0  1 0 0  init
 *  1 0  0 1 0  press up button
 *  0 0  0 1 0  release
 *  1 0  0 0 1  press up button
 *  0 0  0 0 1  release
 *  1 0  0 0 1  press up button  it limits
 *  0 0  0 0 1  release
 *  0 1  0 1 0  press down
 *  0 0  0 1 0  release
 *  0 1  1 0 0  press down
 *
 *
 *  There is a software switch as well that allows the
 *  state to wrap
 *
 *
 *
 * External pin 2 = up
 * External pin 3 = down
 * External pin 5 = Bit0
 * External pin 6 = Bit1
 * External pin 7 = Bit2
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


#define UP_PIN 3
#define DOWN_PIN 4

//Effectively, the debounce delay as well.  If you have flicker problems, increase this.
//If its too long, you will have a latency.
//If you need a number more than 100, get a new switch
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

//Initial state
int state = 0;
//State used to do debounce
boolean debounceUp = LOW;
boolean debounceDown = LOW;


int prevSample1; //timer value


// the setup function runs once when you press reset or power the board
void setup() {
  DDRB = B00000111;
}

void incr () {

  state++;
  if (doWrap == HIGH ) {
    if (state >= 3) {
      state = 0;
    }
  } else {
    if (state >= 3) {
      state = 2;
    }
  }
}

void decr () {

  state--;
  if (doWrap == HIGH ) {
    if (state < 0) {
      state = 2;
    }
  } else {
    if (state < 0) {
      state = 0;
    }
  }

}

void loop() {
  int millsSample = millis();

  //Sample every 10ms
  if ((millsSample - prevSample1) >= LOOP_DELAY ) {
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


    byte output = B00000000;

    if (state == 0) {
      bitWrite(output, 0, HIGH);
    }
    if (state == 1) {
      bitWrite(output, 1, HIGH);
    }
    if (state == 2) {
      bitWrite(output, 2, HIGH);
    }

    PORTB = output;

  }

}
