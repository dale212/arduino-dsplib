//
//  SingelPoleFilter.h
//  
//
//  Created by Dale Heatherington on 2/7/16.
//
//

#ifndef FirstOrderFilter_h
#define FirstOrderFilter_h
#include <stdint.h>

class FirstOrderFilter
{
    public:
    enum filterType{LOWPASS,HIGHPASS};
    
    FirstOrderFilter(FirstOrderFilter::filterType,double,double); //Constructor
    void computeCoefficients( double, double); //Coefficient calculator

    int32_t iterate(int32_t);    //Do the actual filtering. Returns filtered sample.
    uint32_t samplePeriod();    //Return sample rate fs period in microseconds

    private:
    filterType ft;              //LOWPASS or HIGHPASS
    int32_t a0,a1,b1,z1,z2;     //Fixed point coefficients and data storage registers
    double aa0,aa1,bb1;         //Floating point coefficients
    const int32_t q = 14;       // q for fixed point math is 14. (data shifted left by 14 bits)
    uint32_t fsp,t;               //Sample rate period in microseconds = 1000000/sample rate
};

#endif