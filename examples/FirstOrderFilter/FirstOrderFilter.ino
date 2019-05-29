//First Order filter (aka single pole filter) example.
//Basically simulates a simple resistor-capacitor filter.
//Roll off is 6 db/octave
//
//This is designed to run on an Adafruit M0 feather board.
//Note that analogRead() takes 430uS on the M0 so sample rate fs must be under 2 kHz.
//
//
//For testing you need an audio signal generator connected to analog pin A1 with a DC bias of 1.5V
//To observe the output you need an oscilloscope connected the the DAC output on pin A0
//

#include "FirstOrderFilter.h"


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
// 1 filter type: LOWPASS, HIGHPASS
// 2 sample rate in hz
// 3 frequency in hz  (3db cut off  )
// Parameters are type double.  Return values are type int32_t .

FirstOrderFilter hipass = FirstOrderFilter(FirstOrderFilter::HIGHPASS,fs, 20.0);  //20hz highpass
FirstOrderFilter lopass = FirstOrderFilter(FirstOrderFilter::LOWPASS,fs, 100.0);  //100 hz lowpass


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
 *  Analog data is read into x and then processed by high pass and low pass filters.
 *  Becasuse the DC bias is removed by the high pass filter a bias of 512 is added before
 *  sending the final processed data to the DAC.
 */
  
void irqService(){

  x = analogRead(ain);      //read external analog value from ADC.  430uS wasted here.
  
  x = hipass.iterate(x);    //Highpass filter will remove DC bias
  x = lopass.iterate(x);    //Now run it through the low pass filer
  x = x + 512;              //Add bias for the DAC which wants values of 0..1023
 
  analogWrite(dac,x);        //Send filtered data to hardware DAC on A0
  
}

