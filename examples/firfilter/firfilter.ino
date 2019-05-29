
#include "firfilter.h"

//FIR bandpass filter example.

//This is designed to run on an Adafruit M0 feather board.
//Note that analogRead() takes 430uS on the M0 so sample rate fs must be under 2 khz.
//
//For testing you need an audio signal generator connected to analog pin A1 with a DC bias of 1.5V
//To observe the output you need an oscilloscope connected the the DAC output on pin A0

/*

FIR filter designed with
http://t-filter.appspot.com

sampling frequency: 1500 Hz

* 0 Hz - 60 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = -42.046351960983664 dB

* 100 Hz - 150 Hz
  gain = 1
  desired ripple = 1 dB
  actual ripple = 0.5940412405733086 dB

* 190 Hz - 750 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = -42.046351960983664 dB

*/

// Note about using t-filter:  For bandpass filters the width of the 
// upper and lower transition bands must be equal.  They are both 40 Hz in this example.

#define FILTER_TAP_NUM 63

static double filter_taps[FILTER_TAP_NUM] = {
  0.00699181178246915,
  0.006782109677711659,
  0.006982361843043192,
  0.004543399673084658,
  -0.00033624150137469267,
  -0.006287992739589405,
  -0.011190247193650296,
  -0.01305179102986209,
  -0.010988676463978992,
  -0.005810524531553637,
  0.00017154124369958584,
  0.004161237898135317,
  0.004310764676026979,
  0.000882534654340689,
  -0.0035145041792601717,
  -0.00492723095356949,
  -0.000033506496865282615,
  0.011771523107540423,
  0.02724166848934543,
  0.03982085607936052,
  0.04197807536990406,
  0.028572811167166878,
  -0.0002252108116787438,
  -0.03774397951283342,
  -0.07229367965586991,
  -0.09098548832948422,
  -0.08462333660193898,
  -0.05172108454491131,
  0.00006104894155886742,
  0.05588586617753052,
  0.09856385455781515,
  0.11450594822401935,
  0.09856385455781515,
  0.05588586617753052,
  0.00006104894155886742,
  -0.05172108454491131,
  -0.08462333660193898,
  -0.09098548832948422,
  -0.07229367965586991,
  -0.03774397951283342,
  -0.0002252108116787438,
  0.028572811167166878,
  0.04197807536990406,
  0.03982085607936052,
  0.02724166848934543,
  0.011771523107540423,
  -0.000033506496865282615,
  -0.00492723095356949,
  -0.0035145041792601717,
  0.000882534654340689,
  0.004310764676026979,
  0.004161237898135317,
  0.00017154124369958584,
  -0.005810524531553637,
  -0.010988676463978992,
  -0.01305179102986209,
  -0.011190247193650296,
  -0.006287992739589405,
  -0.00033624150137469267,
  0.004543399673084658,
  0.006982361843043192,
  0.006782109677711659,
  0.00699181178246915
};

//Create FIR Bandpass filter 100 Hz to 150 Hz using coefficients above. 

FIRFilter fir = FIRFilter(FILTER_TAP_NUM,filter_taps);

//*** Please connect hardware tonePin to irqPin to make 1500 hz interupts ***
#define irqPin 21
#define tonePin 5

//Analog input pin
#define ain A1
//DAC output pin
#define dac A0

double fs = 1500.0 ;   //Sample rate 1500 hz
volatile int32_t x;



void setup() {
  analogWriteResolution(10);    //10 bit DAC resolution
  analogReadResolution(10);     //10 bit ADC resolution

  tone(tonePin,(int)fs);    //Create 1500 hz interrupts.  (Connect irqPin to tonePin.)

  pinMode(irqPin,INPUT);
  
  attachInterrupt(irqPin, irqService, RISING);

  interrupts();

}

void loop() {
  // not needed

}

/*  All processing is done in this interrupt service routine. 
 *  Analog data is read into x and then processed by high pass, low pass and notch filters.
 *  Becasuse the DC bias is removed by the bandpass filter a bias of 512 is added before
 *  sending the final processed data to the DAC.
 */
void irqService(){

  x = analogRead(ain);      //read external analog value from ADC.  430uS wasted here.
  
  x = fir.iterate(x);      //Apply filter
  x = x + 512;             //Add bias for the DAC which wants values of 0..1023
 
  analogWrite(dac,x);       //Send filtered data to hardware DAC on A0
  
}

