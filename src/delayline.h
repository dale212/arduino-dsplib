//
//  delayline.h
//  
//
//  Created by Dale Heatherington on 2/27/16.
//
//

#ifndef ____delayline__
#define ____delayline__

#include <stdint.h>

class Delayline
{
public:
    Delayline(int32_t);
    int32_t iterate(int32_t);
    int32_t getTap(int32_t);
    
private:
    int32_t *dl ;       //pointer to delay line array
    int32_t length;    //length of array
    int32_t  idx;       //index into aray
    int32_t  *coef;     //Q14 fixed point coefficients array pointer
    
};





#endif /* defined(____delayline__) */
