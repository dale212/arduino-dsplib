//
//  differentiator.cpp
//  
//
//  Created by Dale Heatherington on 2/22/16.
//
//

#include "differentiator.h"

Differentiator::Differentiator(void)
{
    z1 = z2 = z3 = z4 = z5 = z6 = 0;
}

//-------------------------------------------------

int32_t Differentiator::iterate(int32_t in)
{
    int32_t out = (-in + (z2<<4) - (z4<<4) + z6) >> 4;
    
    z6 = z5;
    z5 = z4;
    z4 = z3;
    z3 = z2;
    z2 = z1;
    z1 = in;
    
    return out;
    
}

//----------------------------------------------------

/* End */