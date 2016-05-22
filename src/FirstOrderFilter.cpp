/*
 *  SingelPoleFilter.cpp
 *
 *
 * Created by Dale Heatherington on 2/7/16.
 
 * Some of this code is based on information at earlevel.com.
 * http://www.earlevel.com/main/2012/12/15/a-one-pole-filter/
 
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

#include "FirstOrderFilter.h"
#include <Arduino.h>

/* Fixed point math macros from ARM application note 33 .
 * Most not used .
 */

/* convert a from q1 format to q2 format */
#define FCONV(a, q1, q2) (((q2)>(q1)) ? (a)<<((q2)-(q1)) : (a)>>((q1)-(q2)))


/* The basic operations perfomed on two numbers a and b of fixed
 point q format returning the answer in q format */
#define FADD(a,b) ((a)+(b))
#define FSUB(a,b) ((a)-(b))
#define FMUL(a,b,q) (((a)*(b))>>(q))
#define FDIV(a,b,q) (((a)<<(q))/(b))


/* The basic operations where a is of fixed point q format and b is
 an integer */
#define FADDI(a,b,q) ((a)+((b)<<(q)))
#define FSUBI(a,b,q) ((a)-((b)<<(q)))
#define FMULI(a,b) ((a)*(b))
#define FDIVI(a,b) ((a)/(b))

/* the general operation between a in q1 format and b in q2 format
 returning the result in q3 format */
#define FADDG(a,b,q1,q2,q3) (FCONV(a,q1,q3)+FCONV(b,q2,q3))
#define FSUBG(a,b,q1,q2,q3) (FCONV(a,q1,q3)-FCONV(b,q2,q3))
#define FMULG(a,b,q1,q2,q3) FCONV((a)*(b), (q1)+(q2), q3)
#define FDIVG(a,b,q1,q2,q3) (FCONV(a, q1, (q2)+(q3))/(b))

/* convert to and from floating point */
#define TOFIX(d, q) ((int)( (d)*(double)(1<<(q)) ))
#define TOFLT(a, q) ( (double)(a) / (double)(1<<(q)) )




//------------------------------------------------------------
/* Create filter instance
    Input parameters:
            type:  FirstOrderFilter::LOWPASS or FirstOrderFilter::HIGHPASS
            f0:     3db cutoff frequency
            fc:     Sample rate frequency
 
 */

FirstOrderFilter::FirstOrderFilter(filterType type, double fs,double f0 = 0)
{
    ft = type;
    z1 = z2 = 0;
    fsp = 1000000/fs;   //Sample rate period in microseconds.
    
    computeCoefficients(fs,f0);     //Calculate the coefficients
    
  
    // Convert floating point to Q14 fixed point

    b1 = TOFIX(bb1,q);
    a0 = TOFIX(aa0,q);
    a1 = TOFIX(aa1,q);
 
}

//------------------------------------------------------------------

/* This is the code that does the actual filtering.
 * input:  sampled signal data, 32 bit int.
 * returns filterd signal data
 * z1 is the one sample period delay element
 * a0,a1,b1 are the coefficients
 * This must be called at the sample rate frequency, fs.
 *
 */

int32_t FirstOrderFilter::iterate(int32_t in)
{
    int32_t out=0;
    
   
    switch(ft){
        case LOWPASS:
            out = z1 = ((in * a0) + (z1 * b1)) >> q;    //One pole lowpass
            break;
            
        case HIGHPASS:
            
            z1 = ((in * a0) + (z1 * b1)) >> q;    //One pole lowpass
            out = ((in - z1) * a1) >> q ;         //Convert lowpass to highpass and adjust amplitude
            break;
            
                   
    };
    
    
    
    return out;
}

//----------------------------------------------------------------------

/* Here the coefficients are calculated */

void FirstOrderFilter::computeCoefficients( double fs, double f0)
{
    
    t = TOFIX((4/fs),q);    //Time between samples in Q14 format    
    
    bb1 = exp(-2.0 * M_PI * (f0/fs));
    aa0 = 1.0 - bb1;
    aa1 = 2.0 - bb1;
    
    
    
    
}

//---------------------------------------------------------------------------

/* Returns sample rate period in microseconds for use with micros() timer */

uint32_t FirstOrderFilter::samplePeriod()
{
    
    return fsp;
}

//------------------------------------------------------------------------------







