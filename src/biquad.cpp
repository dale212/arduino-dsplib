/* 
 *  Second order biquad direct form 1 filter library
 *
 *
 *
 *  Created by Dale Heatherington on Jan 24, 2016.
 *
 Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.


*/

#include <Arduino.h>
#include "biquad.h"


//-----------------------------------------------------------------------
/* Create a filter instance
 
   Input parameters:
        type:   Biquad::LOWPASS or Biquad::HIGHPASS or Biquad::BANDPASS or Biquad::NOTCH
        fs:     Sample rate frequency, typically 4x or more times f0.
        f0:     3db cut off frequency for low and high pass. Center frequency for bandpass and notch. 
        Q:      For low and high pass it is 0.7071.  For bandpass and notch it's f0 / 3db bandwidth.
 */

Biquad::Biquad( filterType type, double fs, double f0, double Q)
{
  
 computeCoefficients(type,f0,fs,Q);     //Calculate the coefficients
 
 x1 = 0;            //Zero the memorys
 x2 = 0;
 y1 = 0;
 y2 = 0;
    
    fsp = 1000000/fs;   //Sample rate period in microseconds.
 
 
 
// Convert floating point to q14 fixed point
    b1 = TOFIX(bb1,q);
    b2 = TOFIX(bb2,q);
    a0 = TOFIX(aa0,q);
    a1 = TOFIX(aa1,q);
    a2 = TOFIX(aa2,q);
    
  
}
//-----------------------------------------------------------------------
/* This is the code that does the actual filtering.
 * input:  sampled signal data, 16 bit int.
 * returns filterd signal data
 * This must be called at the sample rate frequency, fs.
 *
 *
 */
 

int32_t Biquad::iterate(int32_t in){
    
    int64_t y0;
    
    y0 = (in*a0) + (x1*a1) + (x2*a2) - (y1*b1) - (y2*b2);   //400ns
    y0 = y0 >> q;
    
    x2 = x1;
    x1 = in;

    y2 = y1;
    y1 = y0;
    
    return y0;
   
}

//--------------------------------------------------------------------------

/* Compute the coefficients
 * This code is based on information at:  http://www.musicdsp.org/files/Audio-EQ-Cookbook.txt
 * and also http://www.earlevel.com/main/2012/11/26/biquad-c-source-code/
 */

void Biquad::computeCoefficients(filterType type, double f0, double fs, double Q)
{
    Qf = Q;
    typef = type;
   double b0;
   double w0 = 2*PI*(f0/fs);
   double cosw0 = cos(w0);
   double sinw0 = sin(w0);
   double alpha = sinw0 / (2*Q);

   if(type == LOWPASS){

    b0 = 1 + alpha;
    bb1 = -2*cosw0/b0;
    bb2 = (1 - alpha) / b0;
    
    aa0 = ((1 - cosw0) / 2)/b0;
    aa1 = (1 - cosw0) / b0 ;
    aa2 = ((1 - cosw0) / 2) / b0;
    
    
   }

   if(type == HIGHPASS){
     b0 = 1 + alpha;
    bb1 = -2*cosw0/b0;
    bb2 = (1 - alpha) /b0;

    aa0 = ((1 + cosw0) / 2)/b0;
    aa1 = -(1 + cosw0) / b0 ;
    aa2 = ((1 + cosw0) / 2) / b0;
   }

   if(type == BANDPASS){
     b0 = 1 + alpha;
    bb1 = -2*cosw0/b0;
    bb2 = (1 - alpha) /b0;

    aa0 =   alpha / b0;
    aa1 =   0 ;
    aa2 =  -alpha / b0;
   }

  if(type == NOTCH){
     b0 = 1 + alpha;
    bb1 = -2*cosw0/b0;
    bb2 = (1 - alpha) / b0;

    aa0 =   1 / b0;
    aa1 =   (-2 * cosw0) / b0;
    aa2 =  1 / b0;
   }
           
        
        
   
   
   
  
}
//---------------------------------------------------------------------------

/* Returns sample rate period in microseconds for use with micros() timer */

uint32_t Biquad::samplePeriod()
{
    
    return fsp;
}

//--------------------------------------------------------------------------
/* print the  coefficients for debug */

void Biquad::print()
{
    
    char s[100];
    char *t = (char*)"error";
    
switch(typef){
    case LOWPASS:   t = (char*)"Lowpass"; break;
    case HIGHPASS:  t = (char*)"Highpass"; break;
    case BANDPASS:  t = (char*)"Bandpass"; break;
    case NOTCH:     t = (char*)"Notch";
};
    
    sprintf(s,"  b1=%2.5f  b2=%2.5f \n a0=%2.5f  a1=%2.5f  a2=%2.5f \n Q=%2.4f",bb1,bb2,aa0,aa1,aa2,Qf);
    Serial.print(t);
    Serial.println(s);
    Serial.println();
    //Serial.println(b1,HEX);
    
  

   
}


