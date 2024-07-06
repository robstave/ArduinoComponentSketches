

/**
 * ACS-85-0751 ATTiny85   I2C Client - Button
 *
 *   I2C Client that transmits the value of a button ( 0 or 255)
 * In this sketch, we are just Sending the value of a pin a over  I2c Bus.
 *  It is waiting for the request really, so its not pushing data, its being polled.
 *
 *  This is a POC thing, although can be a starting point to learning or building something interesting.
 *
 *  Requires the ATTINYCORE library.  That is a good one that implements Wire.h.  So just go with it.
 *  Dont mess with TinyWireM/S  although Im sure it has useful things going for it.  Its mostly that
 *  I could not get it to work any other way 85 <--> 85
 *
 *  Originally, the internet landed me here:
 *  https://thewanderingengineer.com/
 *  It was a good start and Ill have another sketch for rotories that use the code there.
 *
 *  Still my advice is stick to ATTINYCORE for Attiny85 and its included Wire.h
 *
 *
 * Rob Stave (Rob the Fiddler) CCBY 2023
 *
 */
//  ATTiny overview
//                           +-\/-+
//                   Reset  1|    |8  VCC
//      LED Indicator  PB4  2|    |7  PB2 (pin2) SCL
//              Input  PB3  3|    |6  PB1 (pin1) LED Indicator
//                     GND  4|    |5  PB0 (pin0) SDA

#define I2C_CLIENT_ADDRESS 0x4 // Address of the slave

#include <Wire.h>;

volatile int value = 0;

void setup()
{
  Wire.begin(I2C_CLIENT_ADDRESS); // join i2c network
  Wire.onRequest(requestEvent);
  // set pins 3 and 4 to input
  // and enable pullup resisters
  pinMode(3, INPUT);  // Button
  pinMode(4, OUTPUT); // indicator
  pinMode(1, OUTPUT); // indicator
  // Turn on interrupts
}

// Button Attiny receives an i2c request
void requestEvent()
{
  Wire.write(value);
  digitalWrite(4, HIGH); // just a debug
}

void loop()
{

  // read button.  Working with bytes for now
  if (digitalRead(3) == true)
  {
    value = 255;
  }
  else
  {
    value = 0;
  }

  // Indicator (optional)
  if (value > 0)
  {
    digitalWrite(1, HIGH);
  }
  else
  {
    digitalWrite(1, LOW);
  }
}
