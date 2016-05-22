//
//  integrator.h
//  
//
//  Created by Dale Heatherington on 2/22/16.
//
//

#ifndef integrator_h
#define integrator_h

#include <stdint.h>

class Integrator
{
public:
    Integrator(int32_t,int32_t);
    int32_t iterate(int32_t);
    void reset(void);
    
private:
    int32_t low_limit, high_limit, z1;
    
    
};

#endif /* defined(____integrator__) */
