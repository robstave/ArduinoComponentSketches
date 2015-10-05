

//  ATTiny overview
//                           +-\/-+
//                   Reset  1|    |8  VCC
//         (pin3)      PB4  2|    |7  PB2 (pin2)
//         (pin4)      PB3  3|    |6  PB1 (pin1)
//                     GND  4|    |5  PB0 (pin0)


#define VAR1 A3          //Analog read

int prevSample1 = 0;

boolean selectMode = HIGH;
int selectModePin = 4; //


int sampleValue = 0;
#define HYSTERESIS 3




// the setup function runs once when you press reset or power the board
void setup() {
  DDRB = B00000111;
}




void loop() {
  int millsSample = millis();

  if (millsSample - prevSample1 > 50 ) {
    prevSample1 = millsSample;

    selectMode = digitalRead(selectModePin) ;

    int temp = analogRead(VAR1);
    if ( abs(sampleValue - temp) > HYSTERESIS) {
      sampleValue  = temp;
    }





    if (selectMode == HIGH) {

      //Output the value as a byte on PB0,PB1 and PB2
      int outputValue = map(sampleValue, 0, 1023, 0,  7); //Map the slider level to

      byte output  = outputValue & B00000111;
      PORTB = output;
      
    } else {

      int outputValue  = map(sampleValue, 0, 1023, 0,  2); //Map the slider level to

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
