/*

   ACS-85-0125
   Phase Distortion adjustable chainsaw with 1 Volt/Oct

   External pin 1       = Reset (not used)
   External pin 2 (PB3) = Input freq
   External pin 3 (PB4) = Input CV
   External pin 4       = GND
   External pin 5 (PB0) =
   External pin 6 (PB1) = Output (PWM)
   External pin 7 (PB2) =
   External pin 8       = Vcc

   V 1.0  -  First Version ( Fork of ACS-85-121)

   VERY simple version of phase distortion.
   In This case, the normal DDS/Tuning word is used for
   the waveform period.  There is no stretching of the waveform, just
   more of a morphing of a descending ramp to an acending one.

   For this sketch, the phase map is the waveform...but you could easily map that one for one
   to a wavetable

  This is a pretty simple demo, and the frequency math can do with some improvement

     Things that can be improved:
   - analog read is not stellar

  Rob Stave (Rob the fiddler) CCBY 2023

*/

volatile unsigned int Acc;

volatile unsigned int Note = 857;
volatile byte FoldingValue = 128;

// =125*(2^(A1*5/512))
// You can do this in a spreadsheet and then copy/paste to note pad and replace \r with , and \n with ""
// There is room in memory for 1024...but i dont think it will make a lotta difference
const uint16_t offsets[512] PROGMEM = {
  125,126,127,128,128,129,130,131,132,133,134,135,136,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,161,162,163,164,165,166,167,168,170,171,172,173,174,175,177,178,179,180,181,183,184,185,186,188,189,190,191,193,194,195,197,198,199,201,202,204,205,206,208,209,211,212,213,215,216,218,219,221,222,224,225,227,228,230,231,233,235,236,238,239,241,243,244,246,248,249,251,253,254,256,258,260,261,263,265,267,269,270,272,274,276,278,280,282,284,285,287,289,291,293,295,297,299,301,303,305,308,310,312,314,316,318,320,322,325,327,329,331,334,336,338,340,343,345,347,350,352,355,357,359,362,364,367,369,372,374,377,379,382,385,387,390,392,395,398,400,403,406,409,411,414,417,420,423,426,428,431,434,437,440,443,446,449,452,455,459,462,465,468,471,474,478,481,484,487,491,494,497,501,504,508,511,514,518,521,525,529,532,536,539,543,547,550,554,558,562,566,569,573,577,581,585,589,593,597,601,605,609,613,618,622,626,630,635,639,643,648,652,656,661,665,670,674,679,684,688,693,698,702,707,712,717,722,727,731,736,741,746,752,757,762,767,772,777,783,788,793,799,804,810,815,821,826,832,837,843,849,855,860,866,872,878,884,890,896,902,908,915,921,927,933,940,946,952,959,965,972,979,985,992,999,1005,1012,1019,1026,1033,1040,1047,1054,1061,1069,1076,1083,1091,1098,1105,1113,1120,1128,1136,1143,1151,1159,1167,1175,1183,1191,1199,1207,1215,1224,1232,1240,1249,1257,1266,1274,1283,1292,1300,1309,1318,1327,1336,1345,1354,1363,1373,1382,1391,1401,1410,1420,1430,1439,1449,1459,1469,1479,1489,1499,1509,1519,1530,1540,1551,1561,1572,1582,1593,1604,1615,1626,1637,1648,1659,1670,1682,1693,1705,1716,1728,1740,1752,1763,1775,1787,1800,1812,1824,1836,1849,1862,1874,1887,1900,1913,1926,1939,1952,1965,1978,1992,2005,2019,2033,2047,2060,2074,2089,2103,2117,2131,2146,2160,2175,2190,2205,2220,2235,2250,2265,2281,2296,2312,2327,2343,2359,2375,2391,2408,2424,2440,2457,2474,2490,2507,2524,2542,2559,2576,2594,2611,2629,2647,2665,2683,2701,2720,2738,2757,2775,2794,2813,2832,2851,2871,2890,2910,2930,2950,2970,2990,3010,3031,3051,3072,3093,3114,3135,3156,3178,3199,3221,3243,3265,3287,3309,3332,3354,3377,3400,3423,3447,3470,3494,3517,3541,3565,3589,3614,3638,3663,3688,3713,3738,3764,3789,3815,3841,3867,3893,3920,3946,3973
  };


void setup() {
  SetupDDS();
}

void SetupDDS() {
  // Enable 64 MHz PLL and use as source for Timer1
  // PLLCSR = 1<<PCKE | 1<<PLLE;
  // Below is supposed to be better....not sure why
  PLLCSR |= (1 << PLLE);  // PLL enable for asynchronous mode

  while ((PLLCSR & (1 << PLOCK)) == 0x00) {
    // Do nothing until plock bit is set
  }
  PLLCSR |= (1 << PCKE);  // Sets the timer/clock1 source to RC oscillator

  // Set up Timer/Counter1 for PWM output
  TIMSK = 0;                                     // Timer interrupts OFF
  TCCR1 = 1 << PWM1A | 2 << COM1A0 | 1 << CS10;  // PWM A, clear on match, 1:1 prescale
  pinMode(1, OUTPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);

  // Set up Timer/Counter0 for 20kHz interrupt to output samples.
  TCCR0A = 3 << WGM00;              // Fast PWM
  TCCR0B = 1 << WGM02 | 2 << CS00;  // 1/8 prescale
  TIMSK = 1 << OCIE0A;              // Enable compare match, disable overflow
  OCR0A = 60;                       // Divide by 61
}

 

void loop() {
  int temp;

  byte loopCount = 0;
  while (true) {
    //Read freq and map to offset
    temp = map(analogRead(A3), 0, 1023, 0, 511);
    Note = pgm_read_word_near(offsets + temp);
   
    // no need to read cv as often as freq
    if (loopCount % 3 == 0) {
      FoldingValue = map(analogRead(A2), 0, 1023, 0, 255);
    }
    loopCount++;
  }
}

// DDS
ISR(TIMER0_COMPA_vect) {
  Acc = Acc + Note;
  byte ramp = Acc >> 8;

  if (ramp <= FoldingValue) {
    OCR1A = ramp;
  } else {
    OCR1A = 255 - ramp + FoldingValue;
  }
}
