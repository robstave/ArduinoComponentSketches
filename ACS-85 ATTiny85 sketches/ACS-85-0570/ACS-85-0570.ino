/**
   ACS-85-0570
   ATTiny85 PWM Analog Shift Register

   External pin 1       = Reset (not used)
   External pin 2 (PB3) = input 0 freq
   External pin 3 (PB4) = input 1 register count
   External pin 4       = GND
   External pin 5 (PB0) = output output
   External pin 6 (PB1) = input write
   External pin 7 (PB2) = input clock
   External pin 8       = Vcc


  Implements an analog shift register.  Of course, there are not a lot of pins to work with here, so
  you can think of it more like a sample and hold with a delay of x registers?  
  There is a write pin, so sequences can be played back.


  You will need a LPF to convert the pwm to analog.
  a 1k w 2.2uf should work fine in this case (the speed of analog write was boosted at the expense of
  making delay() not usable.  Since its not used...no biggee.
   
   V 1.0  -  First Version


   Rob Stave (Rob the fiddler) ccby 2020
*/

//  ATTiny overview
//                        +-\/-+
//                 Reset 1|    |8  VCC
// (pin3) freq   A3  PB3 2|    |7  PB2 (pin2) clock
// (pin4) count  A2  PB4 3|    |6  PB1 (pin1) write
//                   GND 4|    |5  PB0 (pin0) output
//                        ------

const int int0 = 0; // interrupt 0

volatile byte counter = 0;
volatile unsigned int samples[16];
volatile byte registerSize = 4;
volatile byte sample = 40;
volatile byte registerWrite = 1;

void setup()
{

  DDRB = B00000001; //set output bits

  noInterrupts(); // disable all interrupts
  attachInterrupt(int0, clockCounter, RISING);

  // increase the resolution of analog out to 15k
  // This is at the expense of making delay not work.
  // Fine for this sketch

  TCCR0B = TCCR0B & 0b11111000 | 0b001;

  interrupts(); // enable all interrupts
}

void clockCounter() // called by interrupt
{
  counter++;
  if (counter >= registerSize)
  {
    counter = 0;
  }
  if (registerWrite > 0)
  {
    samples[counter] = sample;
  }

  byte shiftOutCount = 0;

  if (counter == 0)
  {
    shiftOutCount = registerSize - 1;
  }
  else
  {
    shiftOutCount = counter - 1;
  }

  analogWrite(0, samples[shiftOutCount]);
}

byte getRegisterSize(byte sz)
{
  if (sz < 25)
  {
    return 2;
  }
  if (sz < 50)
  {
    return 4;
  }
  if (sz < 75)
  {
    return 8;
  }

  return 16;
}

void loop()
{

  byte loopCounter = 0;

  while (true)
  {

    // Check if we want to go to write mode (usually yes)
    registerWrite = bitRead(PINB, 1);
    // Read value pretty much constantly
    sample = map(analogRead(A3), 0, 1023, 0, 255);

    // only need to read regsiter size every so often
    loopCounter++;
    if (loopCounter > 4)
    {
      loopCounter = 0;
      registerSize = getRegisterSize(map(analogRead(A2), 0, 1023, 0, 100));
    }
  }
}
