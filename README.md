# arduino-dsplib
Simple dsp library for 32 bit Arduinos, such as the Teensy 3.2 or Adafruit M0 feather boards.
It doesn't use any special DSP instructions.  It will probably work on 8 bit boards but will be slow.
However it does require periodic interrupts to iterate dsp code at a fixed rate, for example 22khz.
The IntervalTimer object in the Teensy 3.2 library works well for this.  On other hardware it can be simulated
by using the Tone() function on an output pin that's connected to another input pin defined as an interrupt. Example programs show how  to program this.

The files should be unzipped into a folder named "dsplib" and moved to the path where other Arduino libraries are.  



