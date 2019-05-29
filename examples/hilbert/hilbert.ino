
// Hilbert transform.
// Frequencys .045 to .45 sampling rate are 
// phase shifted -90 degrees with flat amplitude response.
// Both in-phase (I) and quadrature (Q) outputs are available.

// For testing you need an audio signal generator connected to analog pin A1 with a DC bias of 1.5V
// To observe the output you need an 2 channel oscilloscope connected the the DAC output on pin A0
// and digital pin 20.
//

#include "hilbert.h"
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

//Create hilbert transformer
//Returns signal -90 degrees from input.  Also hilbert.getQ()
//Doesn't pass DC.
//Zero degree input signal can be accessed with: hilbert.getI()
//DC is passed in getI().

Hilbert hilbert = Hilbert();   //No parameters

//Create a 5 Hz 1st order lowpass filter for DC blocking
FirstOrderFilter dcblock = FirstOrderFilter(FirstOrderFilter::LOWPASS,fs,5.0);

void setup() {
  analogWriteResolution(10);    //10 bit DAC resolution
  analogReadResolution(10);     //10 bit ADC resolution

  tone(tonePin,(int)fs);    //Create 1500 hz interrupts

  pinMode(20,OUTPUT);
  pinMode(irqPin,INPUT);
  
  attachInterrupt(irqPin, irqService, RISING);

  interrupts();

}

void loop() {
  // Nothing needed here

}


/* All processing is done in this interrupt service routine. 
 * Analog data is read into x and then processed by hilbert filter.
 * 
 * Due to the internal processing delay you will not be able to observe a
 * -90 degree phase shift between the hardware ADC input and the DAC
 * output. However, you can get both I and Q signals in correct phase with  
 * hilbert.getI() and hilbert.getQ().
 */
void irqService(){
  int32_t Q,I;
  
  x = analogRead(ain);      //read external analog value from ADC.  430uS wasted here.
  x = x - dcblock.iterate(x);   // Remove DC bias
  
  Q = hilbert.iterate(x);   // do hilbert transform to get Q

  I = hilbert.getI() ;      //Get I signal. 
  
  if( I >  0)          //We don't have dual DACs so make a square wave for I on pin 20
      digitalWrite(20,HIGH);
    else
      digitalWrite(20,LOW);
      
  Q = Q + 512;              //Add bias to keep DAC in 0..1023 range         
 
  analogWrite(dac,Q);        //Write -90 degree phase shifted data to hardware DAC on A0
  
  
  
}

