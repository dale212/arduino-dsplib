#include "oscillator.h"

/* This open source program makes 500Hz and 1000Hz sine waves on the DAC output pin A14.
 *  It is for the Teensy 3.2 board
 *  It requires IntervalTimer library to create a periodic interrupt
 *  and a hardware DAC.  These are standard on the Teensy 3.2
 *  
 *  The oscillator uses Q14 fixed point math.  For more info about that see:
 *  http://infocenter.arm.com/help/topic/com.arm.doc.dai0033a/DAI0033A_fixedpoint_appsnote.pdf
 */

//-----------------------------------------------------------------------------
const int DAC = A14;    //teensy 3.2 hardware DAC
const int fs = 22000;   //sample rate

IntervalTimer sampleTimer;

Oscillator osc = Oscillator(500,1.0,fs);    //500Hz, Amplitude=1.0, 22000Hz sample rate

void setup() {
  Serial.begin(9600);
  analogWriteResolution(10);
  sampleTimer.begin(procSample,1000000/fs);  //Setup periodic interrupt 22khz
  delay(500);                 //Wait until safe to print
  
  Serial.print(osc.getFreqQ14()); //Print frequency in Q14 fixed point format
  Serial.print(" ");
  Serial.print(osc.getAmpQ14());  //Print amplitude in Q14 fixed point format

}
//------------------------------------------------------------------------------
void procSample()             //Interrupt executes this 22000/sec
{
  int32_t s;
  osc.iterate();              //Iterate the oscillator
  s = osc.getSin();           //Get sine sample.  Or, use getCos() for cosine
  s = s / 32;                 //Scale 16384 peak amplitude down to 512 for DAC
  s = s + 512;                //Add offset for DAC
  analogWrite(DAC,s);         //Write sample to DAC
  
  
}
//---------------------------------------------------------------------------------
void loop() {
  
  delay(1000);
  osc.setFreq(1000);     //Use double or int to set frequency to 1000 hz
  delay(1000);
  osc.setAmp(0.5);      //Set amplitude to 0.5
  delay(1000);
  osc.setFreqQ14(2337);   //Use Q14 fixed point to set frequency to 500hz  (faster!)
  delay(1000);
  osc.setAmp(1.0);        //Set amplitude back to 1.0 .
  
  

}

//--------End-----------------------------------------------
