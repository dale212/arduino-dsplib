
// Sinewave oscillator example
// This code is written for Adafruit M0 feather boards
// Any board with a hardware DAC should work

#include "oscillator.h"

//*** Please connect hardware tonePin to irqPin to make 10,000 hz interupts ***
#define irqPin 21
#define tonePin 5

//Analog input pin
#define ain A1
//DAC output pin
#define dac A0

double fs = 10000 ;   //Sample rate 10 kHz 
volatile int32_t x;

//Create an oscillator
//Parameters
// 1 sample rate
// 2 frequency
// 3 amplitude  (1.0 = 32768 peak to peak)
// Parameter types are double.

Oscillator osc = Oscillator(fs,1000.0,1.0);    //Sample rate 10,000Hz, Freq=1000Hz, Amplitude=1.0, 


void setup() {
  analogWriteResolution(10);    //10 bit DAC resolution
  
  tone(tonePin,(int)fs);    //Create 10,000 hz interrupts.  (Connect irqPin to tonePin.)

  pinMode(irqPin,INPUT);
  
  attachInterrupt(irqPin, irqService, RISING);

  interrupts();

}


//In the main loop we demonstrate changing the frequency and amplitude every second.
void loop() {
  
  osc.setFreq(500.0);       //Set freq to 500 Hz
  osc.setAmp(1.0);          //Ampltude to 1.0
  delay(1000);              //Wait a second
  osc.setFreq(1000.0);       // Set freq to 1000Hz
  osc.setAmp(0.5 );         //Set amplitude to 0.5
  delay(1000);              //Wait a second

}


/*  Oscillator processing is done in this interrupt service routine. 
 */
void irqService(){

  int32_t s;
  osc.iterate();              //Iterate the oscillator
  s = osc.getSin();           //Get sine sample.  Or, use getCos() for cosine
  s = s / 33;                 //Scale 32768 peak to peak amplitude down to 993 for 10 bit DAC
  s = s + 512;                //Add DC bias offset for DAC
  analogWrite(dac,s);         //Write sample to DAC
  
}

