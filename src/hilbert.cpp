//
//  hilbert.cpp
//  
//
//  Created by Dale Heatherington on 2/22/16.
//
//
//  Hilbert filter() executes in about 2.3uS on a 96MHZ Teensy 3.2
//

#include "hilbert.h"


//Macro to convert floating point to fixed point
#define TOFIX(d, q) ((int)( (d)*(double)(1<<(q)) ))


Hilbert::Hilbert(void)
{
    unsigned int i;
    
    for(i=0;i<bufEnd;i++){
        
        z[i] = 0;                       //Zero the ring buffer
    }
    
    bp = 0;
    
    for(i=0;i<NC;i++) coef[i] = TOFIX(coef_f[i],q);  //Convert coefficients to fixed point
    
}



//-----------------------------------------------------------------


//Hilbert transform.
//Frequencys .045 to .45 sampling rate are phase shifted -90 degrees with flat amplitude response.
//
//Uses a 32 word ring buffer so we don't have to move 31 data words.  Just advance pointers instead.
//Buffer size is 32 so we can AND with 31 to handle wrap-around.
//Runs in 2.3 uS on Teensy 3.2.

int32_t Hilbert::iterate(int32_t in)
{
    int i,j,k;
    
    
    
    z[bp] = in;                // Put the input data into the next FIR register location
    
    i = (centerTap + bp -1) & bufEnd;      //Pointer i to the negative coefficients side of the ring buffer
    j = (centerTap + bp +1) & bufEnd;      //Pointer j to positive side
  
    for(k=0;k<NC;k++){                      //Loop for all 8 coefficients
        
        Q += ( (z[j] - z[i]) * coef[k] );   //Do Hilbert transform
        
                                            //Middle out...
        i = (i - 2) & bufEnd;               //Advance i pointer by -2
        j = (j + 2) & bufEnd;               //Advance j pointer by +2
        
    };
    
    k = (bp + centerTap) & bufEnd;      //Point to middle of FIR filter in buffer
    
    I = z[k];                   //Save the 0 degree real part in I  (Use getI() to retrieve)
    
    bp = (bp -1) & bufEnd;      //Point to next buffer location for input
    
    
    Q = Q >> q;             //Convert Q output from fixed point to integer format
                            // Use getQ() to retrieve this later if needed
    
    return Q ;             //Return integer value
    
}



//------------------------------------------------------------------

//Get the original input signal delayed so it leads output by 90 degrees

int32_t Hilbert::getI()
{
    return I;
}

//---------------------------------------------------------------------

//Get the -90 degree Q signal (same as returned by filter() )

int32_t Hilbert::getQ()
{
    return Q;
}

//---------------------------------------------------------------------


