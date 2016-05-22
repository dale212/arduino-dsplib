//
//  hilbert.h
//  
//
//  Created by Dale Heatherington on 2/22/16.
//
//

#ifndef ____hilbert__
#define ____hilbert__

#include <stdint.h>
#include <Arduino.h>
#include <Streaming.h>

class Hilbert
{
public:
    
    Hilbert(void);
    int32_t iterate(int32_t);
    
    int32_t getI(void);
    int32_t getQ(void);
    
private:
    static const int N = 31;        //FIR filter length
    static const int centerTap = (N-1)/2;
    static const int NC = (N+1)/4;  //Number of coefficients
    static const int bufSize = 32;  //bufSize must be a power of 2 and equal or larger than N
    static const int bufEnd = bufSize - 1;   //end of buffer
    
    int32_t z[bufSize];         // z[] is the ring buffer
    int32_t I, Q;               //Outputs.  I is in phase, Q is -90 .
    int bp;                     //ring buffer pointer
    
    const int q = 14;       //Fixed point Q14 format
    
    int32_t coef[NC];        //Fixed point coefficients go in this array
    
    //Hilbert coefficients in floating point format
    
    const double coef_f[NC] = {.6325, .2000, .1078, .0652, .0401, .0238, .0130, .0060};
    
};


#endif /* defined(____hilbert__) */
