//
//  firfilter.cpp
//  
//
//  Created by Dale Heatherington on 2/28/16.
//
//

#include "Firfilter.h"

/* convert to and from floating point */
#define TOFIX(d, q) ((int)( (d)*(double)(1<<(q)) ))
#define TOFLT(a, q) ( (double)(a) / (double)(1<<(q)) )


FIRFilter::FIRFilter(int nc, double coef_f[])
{
    int i;

    ncoef = nc;
    rbl =1;
    
    while(rbl < nc)  rbl = rbl << 1 ;  //Compute size of ring buffer
    
    rbmask = rbl-1;
    rb = new int32_t[rbl];           //Allocate memory to buffer
    
    
    if(rb != NULL){
        for(i=0;i<rbl;i++) rb[i] = 0;     //Clear buffer
    }
    
    idx = 0;                            //Set index to 1st ring buffer location
   
    coef = new int32_t[nc];           //allocate array for fixed point coefficients
 
    if(coef != NULL){
        for(i = 0;i<nc;i++)
            coef[i] = TOFIX(coef_f[i],q) ; //Convert floating point to fixed point
    }
    

}


//---------------------------------------------------------------------------------

int32_t FIRFilter::iterate(int32_t in)
{
    int32_t output = 0;
    int i,j;
    
    rb[idx] = in;
    j = idx;
    
    for( i=0;i<ncoef;i++){
        
        output += coef[i] * rb[j];
        j = (j - 1) & rbmask;
    }
    
    
    idx = (idx+1) & rbmask;
    
        
    return output >> q;
    
    
    
}


//-----------------------------------------------------------------------------------
    