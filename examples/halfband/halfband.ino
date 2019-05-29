//Halfband low pass filter

//This filter is a 31 tap FIR lowpass with cutoff frequency at 1/4 the sample rate.
//Since fs is 1500 Hz cut off is 375 Hz. Roll off is very steep. I measured -24 db at 410 Hz.

//For testing you need an audio signal generator connected to analog pin A1 with a DC bias of 1.5V
//To observe the output you need an oscilloscope connected the the DAC output on pin A0
//


#include "halfband.h"

//*** Please connect hardware tonePin to irqPin to make 1500 hz interupts ***
#define irqPin 21
#define tonePin 5

//Analog input pin
#define ain A1
//DAC output pin
#define dac A0

double fs = 1500.0 ;   //Sample rate 1500 hz
volatile int32_t x;

//Create halfband filter hb
Halfband hb = Halfband();   //No parameters

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
 * Analog data is read into x and then processed by halfband lowpas filter.
 * Cutoff is 1/4 sample frequency fs.
 *  
 */
void irqService(){

  x = analogRead(ain);      //read external analog value from ADC.  430uS wasted here.
  
  x = hb.iterate(x);        //filter data
 
  analogWrite(dac,x);        //Send filtered data to hardware DAC on A0
  
}
