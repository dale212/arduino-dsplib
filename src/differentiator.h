//
//  differentiator.h
//  
//
//  Created by Dale Heatherington on 2/22/16.
//
//

#ifndef ____differentiator__
#define ____differentiator__

#include <stdint.h>

class Differentiator
{
public:
    Differentiator(void);
    int32_t iterate(int32_t);
    
private:
    int32_t z1,z2,z3,z4,z5,z6;
    int32_t const q = 14;
    
};



#endif /* defined(____differentiator__) */
