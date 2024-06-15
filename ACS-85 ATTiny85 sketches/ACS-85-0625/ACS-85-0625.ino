/**
   ACS-85-0625
   ATTiny85  Burst Generator

   External pin 1       = Reset (not used)
   External pin 2 (PB3) = input Burst Period
   External pin 3 (PB4) = input Burst Count
   External pin 4       = GND
   External pin 5 (PB0) = output GATE
   External pin 6 (PB1) = output BURST
   External pin 7 (PB2) = Input 
   External pin 8       = Vcc
   V 1.0  -  First Version

   Rob Stave (Rob the fiddler) ccby 2024
*/


//  ATTiny overview
//                           +-\/-+
//                    Reset 1|    |8  VCC
//     (pin3) Period 2  PB3 2|    |7  PB2 (pin2) input
//      (pin4) Count 3  PB4 3|    |6  PB1 (pin1) gate output
//                      GND 4|    |5  PB0 (pin0) burst output
//                  
        
// Configuration values
//
//   xxx         xxx         xxx         
//   aaabbbbbbbbbaaabbbbbbbbbaaabbbbbbbbb
//   
///  a = trggersize
//   a + b = trigger period

#define TRIGPERIODMIN 30   // min time of the period
#define TRIGPERIODMAX 1000  // max time of the period
#define TRIGGERSIZE 10  // Length of each trigger in the burst

#define GATEPIN PB1
#define OUTPIN PB0
#define INPIN PB2
#define COUNTPIN A2
#define PERIODPIN A3

// the setup function runs once when you press reset or power the board
void setup() {
  DDRB = B00000011;  //set output bits
}

void loop() {

  unsigned long currentMillis;   // now
  unsigned long startBurstTime;  // time in millis when gate started
  unsigned long burstPeriod;  // time in millis of Period

  byte triggerCount = 0;

  // variables
  boolean burstRunning = false;
  boolean inputTrigger = false;
 
  boolean isTriggerOn = false;
 
  digitalWrite(GATEPIN, false);
  digitalWrite(OUTPIN, false);

  while (true) {
     
    inputTrigger = digitalRead(INPIN);

    if (burstRunning == false && inputTrigger) {
      burstRunning = true;
      startBurstTime = millis();
      burstPeriod = map(analogRead(PERIODPIN), 0, 1023, TRIGPERIODMIN, TRIGPERIODMAX);
      triggerCount = map(analogRead(COUNTPIN), 0, 1023, 1, 8);
      isTriggerOn = true;
    }

    currentMillis = millis();

    if (burstRunning && (triggerCount > 0)) {

      digitalWrite(GATEPIN, true);
      digitalWrite(OUTPIN, isTriggerOn);

      if (isTriggerOn) {
        if ((currentMillis - startBurstTime) >= TRIGGERSIZE) {
          isTriggerOn = false;
        }
      } else {
        if ((currentMillis - startBurstTime) >= burstPeriod) {
          isTriggerOn = true;
          triggerCount--;
          startBurstTime = millis();
        }
      }

      if (triggerCount <= 0) {
        burstRunning = false;
        triggerCount=0;
        isTriggerOn = false;
      }
    } else {
      digitalWrite(OUTPIN, false);
      digitalWrite(GATEPIN, false);
    }
  }
}