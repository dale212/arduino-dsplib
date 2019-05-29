//
//  StateVariableFilter.cpp
//  
//
//  Created by Dale Heatherington on 2/8/16.
// Minor bug fix 5-22-19
//
//
/* State variable filter Arduino library code.  Lowpass, highpass, bandpass and band reject outputs are all available at the same time.  Do not set the frequency higher than 1/6 the sample rate.  It will be unstable.
 
 
 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
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
#include "StateVariableFilter.h"

/* Fixed point math macros from ARM application note 33 
 Only TOFIX() used here .
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



//---------------------------------------------------------------------
/* Create an instance of a state variable filter
 Input parameters:
    ft: filter type, LOWPASS,HIGHPASS,BANDPASS,NOTCH
    fs: sample rate frequency
    fc: cutoff frequency  (must not exceed 1/6 sample rate)
    qq:  Filter Q, set to 0.7071 for high abd low pass modes
 
 */


StateVariableFilter::StateVariableFilter(StateVariableFilter::filterType ft,double fs, double fc, double qq=0.7071)
{
    ff = 2*sin((PI*fc)/fs);   //filter frequency tuning
    fq = 1.0/qq;
    
    F = TOFIX(ff,q);
    Q = TOFIX(fq,q);        //Convert double to Q14 fixed point
    
    fsp = 1000000/fs;       //sample period in microseconds
    fsample = fs;
    
    type = ft;
    
    HP=BP=LP=BR=z1a=z1b=0;  //zero all the variables
    
}
//--------------------------------------------------------------------------

//Change the frquency

void StateVariableFilter::setFrequency(double fc)
{
    ff = 2*sin(PI*fc/fsample);
    F = TOFIX(ff,q);
}


//Change the Q

void StateVariableFilter::setQ(double qq)
{
    fq = 1.0/qq ;
    Q   = TOFIX(fq,q);
    
}

//--------------------------------------------------------------------------

/* The actual filter code.  Execute this at the sample rate. 
    Lowpass, highpass, bandpass and band reject outputs are all available.
    Get the outputs from getLP(), getHP(), getBP() or getBR()
 or
    this will return with data associated with the filter type setting.  eg: x = lpf.filter(in);
 */

 int32_t StateVariableFilter::iterate(int32_t in)
{
   
    LP = ((z1a*F)>>q) + z1b ;
    HP = in - ((Q*z1a)>>q)  - LP  ;
    BR = LP + HP;
    BP = ((HP*F)>>q) + z1a ;
    z1a = BP  ;
    z1b = LP ;
    
    
    switch(StateVariableFilter::type){
        case LOWPASS:   return LP ;
        case HIGHPASS:  return HP ;
        case BANDPASS:  return (BP*Q) >> q;
        case NOTCH:     return BR ;
        default:        return 0;
    }
    
    return 0;
    
}

//-----------------------------------------------------------------------------

/* Get filter outputs here.  */

int32_t  StateVariableFilter::getLP()  //Lowpass
{
    return LP;
}

int32_t  StateVariableFilter::getHP()   //Highpass
{
    return HP;
}

int32_t  StateVariableFilter::getBP()   //Bandpass
{
    return (BP*Q) >> q  ;  //mpy by Q to keep BP output level independent of Q
}

int32_t  StateVariableFilter::getBR()   //Band reject
{
    return BR;
}

//--------------------------------------------------------------------------------

uint32_t    StateVariableFilter::samplePeriod()  //Get sample period
{
    return fsp;
}

//---------------------------------------------------------------------------------




    
    
