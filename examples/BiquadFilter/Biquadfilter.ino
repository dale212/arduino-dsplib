//Biquad filter example.
//This is designed to run on an Adafruit M0 feather board.
//Note that analogRead() takes 430uS on the M0 so sample rate fs must be under 2 khz.
//
//This filter type becomes unstable if frequency is set lower than 1/100 the sample frequency fs .
//In this example program fs is 1500 hz so you can't set cutoff freq lower than 15 hz.  Be safe, stay well above.
//If you need lower frequencies try a State Variable Filter.
//
//For testing you need an audio signal generator connected to analog pin A1 with a DC bias of 1.5V
//To observe the output you need an oscilloscope connected the the DAC output on pin A0
//

#include "biquad.h"


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
// 3 frequency in hz  (3db cut off or center freq.  )
// 4 Filter Q.  Must be set to 0.707 for high and low pass filters.
// Parameters are type double.  Return values are type int32_t .

Biquad notch = Biquad(Biquad::NOTCH,fs, 50.0, 10.0);    //50hz band reject (notch) Q = 10
Biquad hipass = Biquad(Biquad::HIGHPASS,fs, 30.0, 0.707); //30 hz high pass
Biquad lopass = Biquad(Biquad::LOWPASS,fs, 100.0,0.707);   //100 hz low pass
Biquad bandpass = Biquad(Biquad::BANDPASS,fs, 60.0,10.0);   //60 hz band pass Q = 10

void setup() {
  analogWriteResolution(10);    //10 bit DAC resolution
  analogReadResolution(10);     //10 bit ADC resolution

  tone(tonePin,(int)fs);    //Create 1500 hz interrupts.  (Connect irqPin to tonePin.)

  pinMode(irqPin,INPUT);
  
  attachInterrupt(irqPin, irqService, RISING);

  interrupts();


}

void loop() {
  //Nothing needed here

}

/*  All processing is done in this interrupt service routine. 
 *  Analog data is read into x and then processed by high pass, low pass and notch filters.
 *  Becasuse the DC bias is removed by the high pass filter a bias of 512 is added before
 *  sending the final processed data to the DAC.
 */
void irqService(){

  x = analogRead(ain);      //read external analog value from ADC.  430uS wasted here.
  
  x = hipass.iterate(x);    //Highpass filter will remove DC bias
  x = lopass.iterate(x);    //Now run it through the low pass filer
  x = notch.iterate(x);     //Remove 50hz with the notch filter

  //x = bandpass.iterate(x);  //uncomment to enable  a band pass filter
  
   x = x + 512;              //Add bias for the DAC which wants values of 0..1023
 
  analogWrite(dac,x);        //Send filtered data to hardware DAC on A0
  
}

