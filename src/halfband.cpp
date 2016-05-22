//
//  halfband.cpp
//  
//
//  Created by Dale Heatherington on 2/26/16.
//
//
// Half band lowpass filter
// Passes DC to 1/4 the sample rate.
// Executes in about 2.2uS on Teensy 3.2

#include "halfband.h"

//Macro to convert floating point to fixed point
#define TOFIX(d, q) ((int)( (d)*(double)(1<<(q)) ))


Halfband::Halfband(void)
{
    unsigned int i;
    
    for(i=0;i<bufEnd;i++){
        
        z[i] = 0;                       //Zero the ring buffer
    }
    
    bp = 0;
    
    for(i=0;i<NC;i++) coef[i] = TOFIX(coef_f[i],q);  //Convert coefficients to fixed point
    
    centerCoef = TOFIX(centerCoef_f,q);
    
}



//-----------------------------------------------------------------




int32_t Halfband::iterate(int32_t in)
{
    int i,j,k;
    output = 0;
    
    
    z[bp] = in;                // Put the input data into the next FIR register location
    
    i = (centerTap + bp -1) & bufEnd;      //Pointer i to the negative coefficients side of the ring buffer
    j = (centerTap + bp +1) & bufEnd;      //Pointer j to positive side
    
    for(k=0;k<NC;k++){                      //Loop for all 8 coefficients
        
        output += ( (z[j] + z[i]) * coef[k] );   //Do filter
        
                                            //Middle out...
        i = (i - 2) & bufEnd;               //Advance i pointer by -2
        j = (j + 2) & bufEnd;               //Advance j pointer by +2
        
    };
    
    k = (bp + centerTap) & bufEnd;      //Point to middle of FIR filter in buffer
    
    output += z[k] * centerCoef;        //Add the center coefficient
    
    bp = (bp -1) & bufEnd;      //Point to next buffer location for input
    
    
    output = output >> q;             //Convert  output from fixed point to integer format
    
    
    return output ;             //Return integer value
    
}

//-------------------------------------------------------------------------------
