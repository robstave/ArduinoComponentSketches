# ACS-85-0705

Tap Tempo Square wave LFO

## Overview

Two buttons provide tap tempo capabilities for two lfo outputs.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0705/images/ACS-85-0705.png)

## Strategy

The loop is read every 10ms.  This allows us set up a two state type machine that we can use to debounce the switch.  Really we are reading the switch and once its high, we take the reading and ignore
everything after that until it is done and we find a different state 10 ms later.  Maybe its a debounce on the way out?
 

## Examples

## Other

Check out [Tap Tempo Electric Druuid](http://electricdruid.net/tap-tempo-lfo-taplfo-v2d/) for a PIC version that he sells to build guitar pedals and such. You can use this sketch as a starting point to get something like that, or just save the time and buy his from the site or  small bear electronics.

[Here is a tremolo](http://www.diystompboxes.com/smfforum/index.php?topic=95586.0) that uses an ardruino for a tap tempo.  He stuffed the whole nano in the box.



