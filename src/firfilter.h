//
//  ifrfilter.h
//  
//
//  Created by Dale Heatherington on 2/28/16.
//
//

#ifndef ____firfilter__
#define ____firfilter__


#include "stdlib.h"
#include "stdint.h"

class FIRFilter
{
public:
    FIRFilter(int, double[]);
    int32_t iterate(int32_t);
    
//private:
    const static int q = 14;    //Q14 fixed point format
    
    int32_t* rb ;       //pointer to ring buffer
    int     rbl;        //ring buffer length
    int     rbmask;      //rbl - 1 for wrap around using &
    int32_t length;    //length FIR filter
    int32_t* coef;      //Fixed point coefficients pointer
    int     ncoef;      //Number of coefficients
    int32_t  idx;       //index into aray

    
    
};


#endif /* defined(____firfilter__) */
