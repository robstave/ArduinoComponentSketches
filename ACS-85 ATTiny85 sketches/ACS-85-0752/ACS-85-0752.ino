

/**
 * ACS-85-0751 ATTiny85   I2C Client - Rotory
 *
 *   I2C Client that transmits the value of a rotery encoder
 *  In this sketch, we are   Sending the value of the rotory over an  I2c Bus.
 *  It is waiting for the request really, so its not pushing data, its being polled.
 *
 *  This is a POC thing, although can be a starting point to learning or building something interesting.
 *
 *  Requires the ATTINYCORE library.  That is a good one that implements Wire.h.  So just go with it.
 *  Dont mess with TinyWireM/S  although Im sure it has useful things going for it.  Its mostly that
 *  I could not get it to work any other way 85 <--> 85
 *
 *  The code for the rotory part came from here
 *  https://thewanderingengineer.com/2013/05/05/rotary-encoder-with-the-attiny85/
 *
 *  Still my advice is stick to ATTINYCORE for Attiny85 and its included Wire.h
 *
 *
 * Rob Stave (Rob the Fiddler) CCBY 2024
 *
 */
//  ATTiny overview
//                           +-\/-+
//                   Reset  1|    |8  VCC
//               Rot1  PB4  2|    |7  PB2 (pin2) SCL
//               Rot2  PB3  3|    |6  PB1 (pin1) LED Indicator
//                     GND  4|    |5  PB0 (pin0) SDA


#include "avr/interrupt.h";
#define I2C_ADDRESS 0x4  // Address of the slave

#include <Wire.h>

volatile int value = 77;
volatile int lastEncoded = 0;

void setup() {
  Wire.begin(I2C_ADDRESS);  // join i2c network
  //TinyWireS.onReceive(receiveEvent); // not using this
  Wire.onRequest(requestEvent);
  // set pins 3 and 4 to input
  // and enable pullup resisters
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(1, OUTPUT);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);

  GIMSK = 0b00100000;  // Enable pin change interrupts
  PCMSK = 0b00011000;  // Enable pin change interrupt for PB3 and PB4
  sei();               // Turn on interrupts
}

// Gets called when the ATtiny receives an i2c request
void requestEvent() {
  Wire.write(value);
  //analogWrite(1, value);  // just a debug
}

void loop() {
  analogWrite(1, value);
}

// This is the ISR that is called on each interrupt
// Taken from http://bildr.org/2012/08/rotary-encoder-arduino/
ISR(PCINT0_vect) {
  int MSB = digitalRead(3);  //MSB = most significant bit
  int LSB = digitalRead(4);  //LSB = least significant bit

  int encoded = (MSB << 1) | LSB;          //converting the 2 pin value to single number
  int sum = (lastEncoded << 2) | encoded;  //adding it to the previous encoded value

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011)
    value++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000)
    value--;

  lastEncoded = encoded;  //store this value for next time

  if (value <= 0)
    value = 0;
  if (value >= 255)
    value = 255;
}