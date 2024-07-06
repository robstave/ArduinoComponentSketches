/**
 * ACS-85-0750 ATTiny85   I2C Master
 *
 *  Control sketch that monitors an I2C Client for a value.
 *
 * In this sketch, we are just monitoring an I2c Bus and outputing a value to a pin ( Digital to start with)
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

#include <Wire.h>  ;

void setup()
{
  pinMode(1, OUTPUT);
  Wire.begin(); // join i2c bus (address optional for master)
}

void loop()
{

  int value = 0;

  while (true)
  {
    Wire.requestFrom(4, 1); // request 1 byte from slave device address 4
    value = Wire.read();    // receive a byte as character
    analogWrite(1, value);
  }
}