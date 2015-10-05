

/**
 * ACS-85-0700 ATTiny85  Analog to Digital Decoder.
 * 
 * Use to convert a pot to digital levels.
 * In Binary mode..it is really just a 3 bit AD converter.
 * The value on the analog pin is converted to 3 bits.
 * {000, 001, 010, ..., 111}
 * 
 * Use to load values into a shift register or counter.
 * A great way to use a pot to control a CD4051 and select
 * an analog channel.
 * 
 * In single select mode, the value converts to one of three values.
 * {001, 010, 100}
 * Can be used to select things as well.
 *
 * External pin 2 = Analog read
 * External pin 3 = mode Binary (high) or single select mode (low)
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
//         Analog in   PB4  2|    |7  PB2 (pin2) Bit2
//              mode   PB3  3|    |6  PB1 (pin1) Bit1
//                     GND  4|    |5  PB0 (pin0) Bit0


#define VAR1 A3          //Analog read

int prevSample1 = 0; //Sample timestamp

boolean selectMode = HIGH;
int selectModePin = 4; //

//A hysteresis of sorts is used to prevent values from switching back and forth.
int sampleValue = 0;
#define HYSTERESIS 3

// the setup function runs once when you press reset or power the board
void setup() {
  DDRB = B00000111;
}


void loop() {
  int millsSample = millis();

  //Sample every 40ms
  if (millsSample - prevSample1 > 40 ) {
    prevSample1 = millsSample;

    selectMode = digitalRead(selectModePin) ;

    int temp = analogRead(VAR1);
    if ( abs(sampleValue - temp) > HYSTERESIS) {
      sampleValue  = temp;
    }

    if (selectMode == HIGH) {

      //Output the value as a byte on PB0,PB1 and PB2
      int outputValue = map(sampleValue, 0, 1023, 0,  7); //Map the slider level to 3 bits

      byte output  = outputValue & B00000111;
      PORTB = output;
      
    } else {

      int outputValue  = map(sampleValue, 0, 1023, 0,  2); //Map the slider level to 3 values

      byte output = B00000000;

      if (outputValue == 0) {
        bitWrite(output, 0, HIGH);
      }
      if (outputValue == 1) {
        bitWrite(output, 1, HIGH);
      }
      if (outputValue == 2) {
        bitWrite(output, 2, HIGH);
      }



      PORTB = output;
    }




  }

}
