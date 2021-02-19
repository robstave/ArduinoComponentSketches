# ACS-85-0210

VCO with 2 tap delays

## Overview:
 
Its a VCO like ACS-85-0041, but the tuning word is stored off and replayed
a little later on.  Kinda like an atari punk console or a shark, it really only works when its in motion.
This will lead to other drone things in the future where things are just constantly modulated.

For now though, its just two taps.

```
   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
   }434|434|455|500|500|500|522|522|533|.......|222|222|222|233|233|233|255|
   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
                   ^             ^                   ^
                  tap           tap1                tuning  --->
                  '
```

 
The Tuning word (note in dds) is stored round robin in the samples array.
To determine port B, we use the tuning word and then bit 1 and bit 2 are just
the spread and spread/2 Mod 127.  It it not PWM, we are just flipping bits.
It sounds better if all bits are set at once.

The size of 127 is chosen because the math is easy...but there is enough memory for 100 more
samples.  Alas...not enough to make it a easy 256 ints.

When mixed, there is some phasing effects.  Consider modulating the phase a bit to make it
mix no matter what.


## Pinout
 

 
 
## Examples

https://soundcloud.com/user-692410397/acs-85-0210-sample



https://soundcloud.com/user-692410397/acs85-0012-0210-a
 