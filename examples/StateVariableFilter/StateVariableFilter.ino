//State variable filter example
//This is designed to run on an Adafruit M0 feather board.
//Note that analogRead() takes 430uS on the M0 so sample rate fs must be under 2 khz.
//
//For testing you need an audio signal generator connected to analog pin A1 with a DC bias of 1.5V
//To observe the output you need an oscilloscope connected the the DAC output on pin A0
//
//This type filter works best when the operating frequency is very low (1/20 or more) relative to the sampling freqency
//It may become unstable if frequency is set higher than 1/6 the sample frequency. 
//If you need higher frequencies try a Biquad filter.


#include "StateVariableFilter.h"


//*** Please connect hardware tonePin to irqPin to make 1500 hz interupts ***
#define irqPin 21
#define tonePin 5

//Analog input pin
#define ain A1
//DAC output pin
#define dac A0

double fs = 1500.0 ;   //Sample rate 1500 hz
volatile int32_t x;

//Create filters.  
//Parameters are;
// 1 filter type: LOWPASS, HIGHPASS, BANDPASS, NOTCH
// 2 sample rate in hz
// 3 frequency in hz  (3db cut off or center freq.  Must not be higher than 1/6 sample rate)
// 4 Filter Q.  Must be set to 0.707 for high and low pass filters.
// Parameters are type double.  Return values are type int32_t .

//Create a 40 hz notch (band reject) filter with a Q of 10
StateVariableFilter notch = StateVariableFilter(StateVariableFilter::NOTCH, fs, 40.0, 10.0);

//Create a 10 hz highpass filter.  
StateVariableFilter hipass = StateVariableFilter(StateVariableFilter::HIGHPASS, fs, 10.0, 0.707);

//Create a 60 hz lowpass filter.  (With 1500 hz sample rate you can't go higher than 250 hz.)
 
StateVariableFilter lopass = StateVariableFilter(StateVariableFilter::LOWPASS, fs, 60.0, 0.707);

//Create a 40 hz bandpass filter with a Q of 6.
StateVariableFilter bandpass= StateVariableFilter(StateVariableFilter::BANDPASS, fs, 40.0, 6.0);



void setup() {
  analogWriteResolution(10);    //10 bit DAC resolution
  analogReadResolution(10);     //10 bit ADC resolution

  tone(tonePin,(int)fs);    //Create 1500 hz interrupts

  pinMode(irqPin,INPUT);
  
  attachInterrupt(irqPin, irqService, RISING);

  interrupts();

}

void loop() {
 //Nothing needed here

}

/* All processing is done in this interrupt service routine. 
 *  Analog data is read into x and then processed by high pass, low pass and notch filters.
 *  Becasuse the DC bias is removed by the high pass filter a bias of 512 is added before
 *  sending the final processed data to the DAC.
 *  
 *  Other outputs can be read from any of the the filters.
 *  Highpass:  filter.getHP();
 *  Lowpass:   filter.getLP();
 *  Bandpass:  filter,getBP();
 *  Notch:     filter.getBR();
 *  
 *  Frequency can be changed.  //f is type double
 *  filter.setFrequency(f);
 *  
 *  The Q can be changed.
 *  filter,setQ(q);     //q is type double  
 *  
 */
void irqService(){

  x = analogRead(ain);      //read external analog value from ADC.  430uS wasted here.
  
  x = hipass.iterate(x);    //Highpass filter will remove DC bias
  x = lopass.iterate(x);    //Now run it through the low pass filer
  x = notch.iterate(x);     //Remove 40hz with the notch filter

  //x = bandpass.iterate(x);  //uncomment to enable  a band pass filter
  
   x = x + 512;              //Add bias for the DAC which wants values of 0..1023
 
  analogWrite(dac,x);        //Send filtered data to hardware DAC on A0
  
}

