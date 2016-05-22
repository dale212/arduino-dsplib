//
//  oscillator.cpp
//  
//
//  Created by Dale Heatherington on 2/10/16.
//
//  State variable oscillator based on info at
//  http://www.earlevel.com/main/2003/03/02/the-digital-state-variable-filter/



/*
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
#include "oscillator.h"

/* convert fixed point to and from floating point.  q is set to 14 in oscillator.h */
#define TOFIX(d, q) ((int)( (d)*(double)(1<<(q)) ))
#define TOFLT(a, q) ( (double)(a) / (double)(1<<(q)) )
/* See:  http://infocenter.arm.com/help/topic/com.arm.doc.dai0033a/DAI0033A_fixedpoint_appsnote.pdf
 */

//----------------------------------------------------
/* Create oscillator instance
 
 input parameters:
        fs:     Sampling rate in hz
        freq:  frequency in HZ
        Amp:    amplitude, typically 1.0
 
 
 */

Oscillator::Oscillator(double fs, double freq, double amp )
{
    sampleRate = fs;
    double ff = 2*sin((PI*freq)/sampleRate);
    a = TOFIX(amp,q);
    f = TOFIX(ff,q);
    sinZ = 0;
    cosZ = a;
    
    
    
}

//---------------------------------------------------------

//Call this at the sampling rate.

void Oscillator::iterate()
{
    sinZ = sinZ + ((f * cosZ) >> q);
    cosZ = cosZ - ((f * sinZ) >> q);
    
    
    
}

//------------------------------------------------------------

//Get the current cosine wave data point

int32_t Oscillator::getCos()
{
    
    return  cosZ;
}

//Get the current sine wave data point

int32_t Oscillator::getSin()
{
    return sinZ;
}

//-------------------------------------------------------------

//Set frequency in Hz

void    Oscillator::setFreq(double freq)
{
    freq = 2*sin((PI*freq)/sampleRate);
    f = TOFIX(freq,q);
}

//----------------------------------------------------------------

//Set frequency directly as fo/fs << 14

void    Oscillator::setFreqQ14(int32_t freq)
{
    f = freq;
}

//-------------------------------------------------------------------

int32_t Oscillator::getFreqQ14(void)
{
    return f;
}

//-------------------------------------------------------------------

void    Oscillator::setAmp(double amp)
{
    a = TOFIX(amp,q);
    sinZ = 0;
    cosZ = a;
}

//-----------------------------------------------------------------------

int32_t  Oscillator::getAmpQ14(void)
{
    return a;
}

//-----------------------------------------------------------------------
//end of file


