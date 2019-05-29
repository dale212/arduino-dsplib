
//Delay Line example
//This is designed to run on an Adafruit M0 feather board.
//Note that analogRead() takes 430uS on the M0 so sample rate fs must be under 2 khz.
//
//For testing you need an audio signal generator connected to analog pin A1 with a DC bias of 1.5V
//To observe the output you need an oscilloscope connected the the DAC output on pin A0
//

#include "delayline.h"

//*** Please connect hardware tonePin to irqPin to make 1500 hz interupts ***
#define irqPin 21
#define tonePin 5

//Analog input pin
#define ain A1
//DAC output pin
#define dac A0

double fs = 1500.0 ;   //Sample rate 1500 hz
volatile int32_t x;

//Create delay line
//Parameter is length in samples and type int32_t.
//Return value is type int32_t delayed by "length" samples.
//Signal can be read from any tap with dl.getTap(N) after dl.iterate(x);

Delayline dl = Delayline(15);    // 15 clocks delay is 10ms with fs=1500

void setup() {
  analogWriteResolution(10);    //10 bit DAC resolution
  analogReadResolution(10);     //10 bit ADC resolution

  tone(tonePin,(int)fs);    //Create 1500 hz interrupts.  (Connect irqPin to tonePin.)

  pinMode(irqPin,INPUT);
  
  attachInterrupt(irqPin, irqService, RISING);

  interrupts();



}

void loop() {
  // Not needed in this example

}

/*  All processing is done in this interrupt service routine. 
 *  Analog data is read into x and then processed by delay line.
 */
void irqService(){

  x = analogRead(ain);      //read external analog value from ADC.  430uS wasted here.
  
  x = dl.iterate(x);        // Get the delayed signal
  
  analogWrite(dac,x);        //Send delayed signal to hardware DAC on A0
                             
}


