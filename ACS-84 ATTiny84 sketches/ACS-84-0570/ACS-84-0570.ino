/**
   ACS-85-0608

    External pin 1       = Vcc
   External pin 2  (PB0) = not used
   External pin 3  (PB1) = not used
   External pin 4  (PB)  = Reset (not used)
   External pin 5  (PB0) = not used
   External pin 6  (PA7) = not used
   External pin 7  (PA6) = output clock as trigger
   External pin 8  (PA5) = out 1
   External pin 9  (PA4) = out 2
   External pin 10 (PA3) = out 3
   External pin 11 (PA2) = out 4
   External pin 12 (PA1) = Trigger Length
   External pin 13 (PA0) = Random
   External pin 14       = GND
   V 1.0  -  First Version

   Rob Stave (Rob the fiddler) ccby 2020
*/

//https://forum.arduino.cc/index.php?topic=214375.0

//  ATTiny overview
//                        +-\/-+
//               VCC set 1|    |8  GND
//                   PB0 2|    |7  PA0 clock
//                   PB1 3|    |6  PA1 Input
//                 reset 4|    |5  PA2 select 4/8
//          output 4 PB2 5|    |5  PA3  write
//          output 3 PA7 6|    |5  PA4  
//          output 2 PA6 7|    |5  PA5 output 1
//                        ------


volatile byte counter = 0;
volatile unsigned int samples[8];
volatile byte registerSize = 4;
volatile  byte sample = 40;
volatile  byte registerWrite = 1;


//https://electronics.stackexchange.com/questions/50380/determining-which-pin-triggered-a-pcintn-interrupt

volatile uint8_t portbhistory = 0xFF;     // default is high because the pull-up
void setup() {
  DDRA = B00010000;  //set output bits
  noInterrupts(); // disable all interrupts

  TCCR0B = TCCR0B & 0b11111000 | 0b001;
  TCCR1B = TCCR1B & 0b11111000 | 0b001;


  PORTA |= ((1 << PORTA0) | (1 << PORTA2) | (1 << PORTA3) | (1 << PORTA4));
  // PA0are now inputs with pull-up enabled

  PCMSK0 = bit (PCINT0) ;
  GIMSK  = bit (PCIE0);
  interrupts(); // enable all interrupts

}


ISR (PCINT0_vect)
{
  uint8_t changedbits;

  changedbits = PINA ^ portbhistory;
  portbhistory = PINA;

  if (changedbits & (1 << PA0))
  {
    /* PCINT0 changed */
    PORTA ^= (_BV(PA4));
    clockCounter();
  }


}



void clockCounter() // called by interrupt
{
  counter++;
  if (counter >= registerSize) {
    counter = 0;
  }

  if (registerWrite > 0)  {
    samples[counter] = sample;
  }

  byte shiftOutCount = 0;

  if (counter == 0){
    shiftOutCount = registerSize - 1;
  }  else  {
    shiftOutCount = counter - 1;
  }

  analogWrite(5, samples[shiftOutCount]);
  analogWrite(6, samples[((shiftOutCount - 1 + registerSize) % registerSize)]);
  analogWrite(7, samples[((shiftOutCount - 2 + registerSize) % registerSize)]);
  analogWrite(8, samples[((shiftOutCount - 3 + registerSize) % registerSize)]);
}



void loop() {

  byte loopCounter = 0;
  byte regSize = 0;
  while (true)
  {

    // Check if we want to go to write mode (usually yes)
    registerWrite = bitRead(PINA, 3);
    regSize = bitRead(PINA, 2);
    if (regSize > 0) {
      registerSize = 8;
    } else {
      registerSize = 4;
    }

    // Read value pretty much constantly
    sample = map(analogRead(A1), 0, 1023, 0, 255);



  }
}
