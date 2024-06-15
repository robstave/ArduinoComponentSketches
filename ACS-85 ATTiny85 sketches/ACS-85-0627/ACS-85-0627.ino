/**
   ACS-85-0627
   ATTiny85  Burst Generator rando

   External pin 1       = Reset (not used)
   External pin 2 (PB3) = input Burst Period
   External pin 3 (PB4) = input probabilty
   External pin 4       = GND
   External pin 5 (PB0) = output 
   External pin 6 (PB1) = output 
   External pin 7 (PB2) = Input 
   External pin 8       = Vcc
   V 1.0  -  First Version

   Rob Stave (Rob the fiddler) ccby 2024
*/


//  ATTiny overview
//                           +-\/-+
//                    Reset 1|    |8  VCC
//     (pin3) Period 2  PB3 2|    |7  PB2 (pin2) input
//      (pin4) prob 3   PB4 3|    |6  PB1 (pin1)  output
//                      GND 4|    |5  PB0 (pin0)  output
//



#define TRIGPERIODMIN 20   // min time of the period
#define TRIGPERIODMAX 900  // max time of the period
#define TRIGGERSIZE 10     // Length of each trigger in the burst

#define OUTPIN2 PB1
#define OUTPIN1 PB0
#define INPIN PB2
#define PROBPIN A2
#define PERIODPIN A3


const static byte burst0[2][1] PROGMEM = {
  { 1 },
  { 1 }
};

const static byte burst1[2][2] PROGMEM = {
  { 1, 1 },
  { 0, 1 }
};

const static byte burst2[2][3] PROGMEM = {
  { 1, 1, 1 },
  { 0, 1, 1 }
};


const static byte burst3[2][4] PROGMEM = {
  { 1, 1, 1, 1 },
  { 0, 1, 0, 1 }
};

const static byte burst4[2][5] PROGMEM = {
  { 1, 1, 1, 1, 1 },
  { 0, 0, 0, 1, 1 }
};

const static byte burst5[2][6] PROGMEM = {
  { 1, 1, 1, 1, 1, 1 },
  { 0, 1, 0, 1, 0, 1 }
};

const static byte burst6[2][8] PROGMEM = {
  { 1, 1, 1, 1, 1, 1, 1, 1 },
  { 0, 1, 1, 1, 0, 1, 1, 1 }
};

struct BurstData {
  int size;
  bool value;
};

BurstData calcBurstData(int scene, int index, int triggerCount) {
  BurstData data;

  if (scene == 0) {
    data.size = sizeof(burst0[0]);
    data.value = (pgm_read_byte(&(burst0[index][data.size - triggerCount]))) == 1;
  } else if (scene == 1) {
    data.size = sizeof(burst1[0]);
    data.value = (pgm_read_byte(&(burst1[index][data.size - triggerCount]))) == 1;
  } else if (scene == 2) {
    data.size = sizeof(burst2[0]);
    data.value = (pgm_read_byte(&(burst2[index][data.size - triggerCount]))) == 1;
  } else if (scene == 3) {
    data.size = sizeof(burst3[0]);
    data.value = (pgm_read_byte(&(burst3[index][data.size - triggerCount]))) == 1;
  } else if (scene == 4) {
    data.size = sizeof(burst4[0]);
    data.value = (pgm_read_byte(&(burst4[index][data.size - triggerCount]))) == 1;
  } else if (scene == 5) {
    data.size = sizeof(burst5[0]);
    data.value = (pgm_read_byte(&(burst5[index][data.size - triggerCount]))) == 1;
  } else if (scene == 6) {
    data.size = sizeof(burst6[0]);
    data.value = (pgm_read_byte(&(burst6[index][data.size - triggerCount]))) == 1;
  } else {
    data.size = 4;
    data.value = false;
  }

  return data;
}

// the setup function runs once when you press reset or power the board
void setup() {
  DDRB = B00000011;  //set output bits
}

void loop() {

  unsigned long currentMillis;   // now
  unsigned long startBurstTime;  // time in millis when gate started
  unsigned long burstPeriod;     // time in millis of Period

  byte triggerCount = 0;
  int scene = 0;

  // variables
  boolean burstRunning = false;
  boolean inputTrigger = false;
  boolean isTriggerOn = false;

  digitalWrite(OUTPIN1, false);
  digitalWrite(OUTPIN2, false);

  while (true) {

    inputTrigger = digitalRead(INPIN);

    if (burstRunning == false && inputTrigger) {
      burstRunning = true;
      startBurstTime = millis();
      burstPeriod = map(analogRead(PERIODPIN), 0, 1023, TRIGPERIODMIN, TRIGPERIODMAX);

      int prob = map(analogRead(PROBPIN), 0, 1023, 0, 99);

      isTriggerOn = true;

      int r = random(5, 99);

      scene = 0;

      // if the prob hits, select a random scene
      if (r < prob) {
        scene = random(1, 6);
      }

      BurstData bd = calcBurstData(scene, 0, 1);

      triggerCount = bd.size;
    }

    currentMillis = millis();

    if (burstRunning && (triggerCount > 0)) {

      if (isTriggerOn) {
        BurstData p1 = calcBurstData(scene, 0, triggerCount);
        BurstData p2 = calcBurstData(scene, 1, triggerCount);



        digitalWrite(OUTPIN1, p1.value);
        digitalWrite(OUTPIN2, p2.value);
      } else {
        digitalWrite(OUTPIN1, false);
        digitalWrite(OUTPIN2, false);
      }


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
        triggerCount = 0;
        isTriggerOn = false;
      }
    } else {
      digitalWrite(OUTPIN1, false);
      digitalWrite(OUTPIN2, false);
    }
  }
}