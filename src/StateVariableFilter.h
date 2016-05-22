//
//  StateVariableFilter.hpp
//  
//
//  Created by Dale Heatherington on 2/8/16.
//
//

#ifndef StateVariableFilter_h
#define StateVariableFilter_h



#include "StateVariableFilter.h"

class   StateVariableFilter
{
public:
    enum filterType { LOWPASS,HIGHPASS,BANDPASS,NOTCH};
    
    StateVariableFilter(StateVariableFilter::filterType,double,double,double);  //Constructor (freq,sample rate, Q)
    int32_t     iterate(int32_t);               //Do filter iteration
    void        setFrequency(double);           //Set filter frequency
    void        setQ(double);                   //set filter Q
    int32_t     getBP(void);                    //Get bandpass filter data
    int32_t     getLP(void);                    //Get lowpass filter data
    int32_t     getHP(void);                    //Get hightpass filter data
    int32_t     getBR( void);                   //Get band reject filter data
    uint32_t    samplePeriod();                 //Get sample period in microseconds
    
private:
    const int32_t q = 14;   // q for fixed point math is 14   (not related to filter Q )
    double  ff,fq;          //filter frequency and Q coefficients
    int64_t z1a,z1b,F,Q;    // Fixed point delay and filter coefficients used at run time
    int64_t HP,BP,LP,BR;    // Fixed point  Filter outputs
    uint32_t fsp;           //Sample rate period in microseconds
    uint32_t fsample;        //Sample rate in hz
    filterType type;         //
    
};


#endif /* StateVariableFilter_h */
