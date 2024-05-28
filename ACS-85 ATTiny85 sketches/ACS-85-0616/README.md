# ACS-85-0616

Probability gate with fixed length and probs.  Variable delay of the trigger afterwards.

## Overview

A chance sketch with the trigger on Pin 7.
On rising, the output of Pin 5 and 6 trigger for a fixed Length with a 50% and 25% probability.  
   
The Probability and trigger length values are fixed.

If PB4 is High, the the gate triggers after a delay ( the lenght remains the same).
If Pin 3 is low ( not plugged in, you may need to tie that down) then it just acts like a regular probability gate.
If the pin is high, then an added delay happens before the pin is set to high.

A3 determines the length of the delay.

## Output

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0616/images/ACS-85-0616.png)

## Notes

![cat](https://lh3.googleusercontent.com/pw/AP1GczNrxXCGrzwhsksV_zmo2kYkiovtFc7uNljgMNTNafme_zlj16Z8Sgpq1_JSBkHPmOvkyarz-uaH1Qjma065DdcSaLx0vXZNjCBxoLw0jQFDQ5ueUok=w2400)
 