//
//  halfband.h
//  
//
//  Created by Dale Heatherington on 2/26/16.
//
//Half band lowpass filter
//Passes DC to 1/4 the sample rate.

#ifndef ____halfband__
#define ____halfband__

#include <stdint.h>

class Halfband
{
public:
    
    Halfband(void);
    int32_t iterate(int32_t);
    
    
    
    //private:
    static const int N = 31;        //FIR Halfband filter length
    static const int centerTap = (N-1)/2;
    static const int NC = (N+1)/4;  //Number of coefficients
    static const int bufSize = 32;  //bufSize must be a power of 2 and equal or larger than N
    static const int bufEnd = bufSize - 1;   //end of buffer
    
    int32_t z[bufSize];         // z[] is the ring buffer
    int32_t output  ;           //Output
    int bp;                     //ring buffer pointer
    
    const int q = 14;       //Fixed point Q14 format
    
    int32_t coef[NC];       //Fixed point coefficients in this array
    int32_t centerCoef;     //Fixed point center coefficient
    
    //Halfband lowpass coefficients
    
    const double coef_f[NC] = {.31692,-.10199,.05699,-.03648,.02437,-.01632,.01066,-.01023};
    const double centerCoef_f = 0.5;
    
    
};



#endif /* defined(____halfband__) */
