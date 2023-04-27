#include <avr/io.h>
#include <avr/interrupt.h>


/**
 * ACS-85-0034
 * Lorenz attractor LFO
 *
 * I have not tried this on a scope ( need another probe for that)
 * but the outputs are consistant with the attractor.
 *
 * its very pixelated sounding but can be used to create two
 * LFO CV signals.
 * 
 * DO play with things

 *
 * External pin 1       = Reset (not used)
 * External pin 2 (PB3) = Freq
 * External pin 3 (PB4) = nc
 * External pin 4       = GND
 * External pin 5 (PB0) = X
 * External pin 6 (PB1) = Y
 * External pin 7 (PB2) = nc
 * External pin 8       = Vcc

//  ATTiny overview
//                      +-\/-+
//                 set 1|    |8  VCC
// (pin3) freq     PB3 2|    |7  PB2 (pin2/int0) nc
// (pin4) nc       PB4 3|    |6  PB1 (pin1) y PWM
//                 GND 4|    |5  PB0 (pin0) x PWM
//                          ------


 *
 * V 1.0  -  First Version
 * 
 *
 * Rob Stave (Rob the fiddler) ccby 2023
 */

                         ------

// For Timer1 millis
#define F_CPU 8000000UL  // 8 MHz clock speed
#define TIMER1_PRESCALER 64
#define TIMER1_TICKS_PER_MS (F_CPU / 1000 / TIMER1_PRESCALER)

#define CV_PIN A3
#define PWM_X_PIN 0
#define PWM_Y_PIN 1

// Lorenz Attractor parameters
const float dt = 0.01;
const float a = 10.0;
const float b = 28.0;
const float c = 8.0 / 3.0;

void setup() {
  pinMode(CV_PIN, INPUT);
  pinMode(CLOCK_DEBUG, OUTPUT);
  pinMode(PWM_X_PIN, OUTPUT);
  pinMode(PWM_Y_PIN, OUTPUT);
  
  noInterrupts();  // disable all interrupts
  

  // juice the analog out
  TCCR0B = TCCR0B & 0b11111000 | 0b001;

  // it messes with delay though..so use timer1
  TCCR1 = (1 << CTC1) | (7 << CS10);  // CTC  mode, div64
  OCR1C = 0.001 * F_CPU / 64 - 1;     // 1ms, F_CPU @16MHz, div64
  TIMSK |= (1 << OCIE1A);

  interrupts();  // enable all interrupts
}

// Use the second timer as our "timer"
volatile unsigned long milliseconds;

void delay1(uint16_t ms) {
  unsigned long currentMillis = millisTimer1();
  while ((millisTimer1() - currentMillis) < ms) {
    // Wait for the desired time to elapse
  }
  milliseconds = 0;
}

// Set second timer to implement "millis"
ISR(TIMER1_COMPA_vect) {
  milliseconds++;
}

unsigned long millisTimer1() {
  return milliseconds;
}

void loop() {

  uint8_t pwmXValue;
  uint8_t pwmYValue;

  float x = 0.1;
  float y = 0;
  float z = 0;

  while (true) {

    uint16_t cvValue = analogRead(CV_PIN);
    uint16_t interval = map(cvValue, 0, 1023, 10, 400);

    // Update the Lorenz Attractor
    float dx = a * (y - x) * dt;
    float dy = (x * (b - z) - y) * dt;
    float dz = ((x * y) - (c * z)) * dt;

    x = x + dx;
    y = y + dy;
    z = z + dz;

    // Scale the Lorenz Attractor values to 8-bit PWM
    pwmXValue = map(constrain(x + x / 2, -30, 30), -30, 30, 0, 255);
    //pwmYValue = map(constrain(y + y / 2, -30, 30), -30, 30, 0, 255);
    pwmYValue = map(constrain(y + y / 2, -30, 30), -30, 30, 255, 0);

    // Wait for the next LFO update
    analogWrite(PWM_X_PIN, pwmXValue);
    analogWrite(PWM_Y_PIN, pwmYValue);

    delay1(interval);
  }
}
