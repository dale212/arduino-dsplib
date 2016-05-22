//
//  integrator.cpp
//  
//
//  Created by Dale Heatherington on 2/22/16.
//
//

#include "integrator.h"

Integrator::Integrator(int32_t ll, int32_t hl)
{
    low_limit = ll;
    high_limit = hl;
    z1 = 0;
}

//--------------------------------------------------------------

int32_t Integrator::iterate(int32_t in)
{
    
    z1 = in + z1;
    
    if(z1 > high_limit) z1 = high_limit;
    if(z1 < low_limit)  z1 = low_limit;
    return z1;
    
}

//----------------------------------------------------------------

void Integrator::reset(void)
{
    z1 = 0;
    
}

//-----------------------------------------------------------------

/* End */

