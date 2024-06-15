/**
   ACS-85-0626
   ATTiny85  Burst Generator 2 pin

   External pin 1       = Reset (not used)
   External pin 2 (PB3) = input Burst Period
   External pin 3 (PB4) = input scene
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
//      (pin4) scene 3  PB4 3|    |6  PB1 (pin1)  output
//                      GND 4|    |5  PB0 (pin0)  output
//

#define TRIGPERIODMIN 100  // min time of the period
#define TRIGPERIODMAX 1000 // max time of the period
#define TRIGGERSIZE 10     // Length of each trigger in the burst

#define OUTPIN2 PB1
#define OUTPIN1 PB0
#define INPIN PB2
#define COUNTPIN A2
#define PERIODPIN A3

const static byte burst0[2][4] PROGMEM = {
    {1, 1, 1, 1},
    {0, 1, 0, 1}};

const static byte burst1[2][8] PROGMEM = {
    {1, 1, 1, 1, 1, 1, 1, 1},
    {0, 1, 1, 1, 0, 1, 1, 1}};

int calcBurstSize(int scene)
{
    if (scene == 0)
    {
        return 4;
    }
    if (scene == 1)
    {
        return 8;
    }
    return 4;
}

bool calcBurst(int scene, int index, int triggerCount)
{

    if (scene == 0)
    {
        return (pgm_read_byte(&(burst0[index][calcBurstSize(scene) - triggerCount]))) == 1;
    }

    if (scene == 1)
    {
        return (pgm_read_byte(&(burst1[index][calcBurstSize(scene) - triggerCount]))) == 1;
    }
}

// the setup function runs once when you press reset or power the board
void setup()
{
    DDRB = B00000011; // set output bits
}

void loop()
{

    unsigned long currentMillis;  // now
    unsigned long startBurstTime; // time in millis when gate started
    unsigned long burstPeriod;    // time in millis of Period

    byte triggerCount = 0;
    int scene = 0;

    // variables
    boolean burstRunning = false;
    boolean inputTrigger = false;
    boolean isTriggerOn = false;

    digitalWrite(OUTPIN1, false);
    digitalWrite(OUTPIN2, false);

    while (true)
    {

        inputTrigger = digitalRead(INPIN);

        if (burstRunning == false && inputTrigger)
        {
            burstRunning = true;
            startBurstTime = millis();
            burstPeriod = map(analogRead(PERIODPIN), 0, 1023, TRIGPERIODMIN, TRIGPERIODMAX);

            scene = map(analogRead(COUNTPIN), 0, 1023, 0, 1);
            triggerCount = calcBurstSize(scene);

            isTriggerOn = true;
        }

        currentMillis = millis();

        if (burstRunning && (triggerCount > 0))
        {

            if (isTriggerOn)
            {
                boolean p1 = calcBurst(scene, 0, triggerCount);
                boolean p2 = calcBurst(scene, 1, triggerCount);

                digitalWrite(OUTPIN1, p1);
                digitalWrite(OUTPIN2, p2);
            }
            else
            {
                digitalWrite(OUTPIN1, false);
                digitalWrite(OUTPIN2, false);
            }

            if (isTriggerOn)
            {
                if ((currentMillis - startBurstTime) >= TRIGGERSIZE)
                {
                    isTriggerOn = false;
                }
            }
            else
            {
                if ((currentMillis - startBurstTime) >= burstPeriod)
                {
                    isTriggerOn = true;
                    triggerCount--;
                    startBurstTime = millis();
                }
            }

            if (triggerCount <= 0)
            {
                burstRunning = false;
                triggerCount = 0;
                isTriggerOn = false;
            }
        }
        else
        {
            digitalWrite(OUTPIN1, false);
            digitalWrite(OUTPIN2, false);
        }
    }
}