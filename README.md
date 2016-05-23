# arduino-dsplib
Simple dsp library for 32 bit Arduinos, the Teensy 3.2 in particular.
It doesn't use any special DSP instructions.
However it does require periodic interrupts to iterate dsp code at a fixed rate, for example 22khz.
The IntervalTimer object in the Teensy 3.2 library works well for this.  On other hardware it can be simulated
by using the Tone() function on an output pin that's connected to another input pin defined as an interrupt.

The files should be unzipped into a folder named "dsplib" and moved to the path where other Arduino libraries are.  



Classes:

Biquad( filterType type, double fs, double f0, double Q)
      iterate(int32_t in)
      samplePeriod()
      print()
      

Delayline(int32_t N)
      iterate(int32_t in)
      getTap(int32_t N)
      

Differentiator(void)
      iterate(int32_t in)
      
      

FIRFilter(int nc, double coef_f[])
      iterate(int32_t in)
      
      

FirstOrderFilter(filterType type, double fs,double f0 = 0)
      iterate(int32_t in)
      samplePeriod()
      
      

Halfband(void)
      iterate(int32_t in)
      
      

Hilbert(void)
      iterate(int32_t in)
      getI()
      getQ()
      
      

Integrator(int32_t ll, int32_t hl)
      iterate(int32_t in)
      reset(void)
      

Oscillator(double fs, double freq, double amp )
      iterate()
      getCos()
      getSin()
      setFreq(double freq)
      setFreqQ14(int32_t freq)
      getFreqQ14(void)
      setAmp(double amp)
      getAmpQ14(void)
      
      

StateVariableFilter(StateVariableFilter::filterType ft,double fs, double fc, double qq=0.7071)
      setFrequency(double fc)
      setQ(double qq)
      iterate(int32_t in)
      getLP()
      getLP()
      getBP()
      getBR()
      samplePeriod()
      
      
      More documentation to come...



