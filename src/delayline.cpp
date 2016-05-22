//
//  delayline.cpp
//  
//
//  Created by Dale Heatherington on 2/27/16.
//
//

#include "delayline.h"


//Create new delay line object with user defined length
//This is implemented with a dynamically allocated circular buffer.

Delayline::Delayline(int32_t N)
{
    length = N;                     //Set length of array
    dl = new int32_t[N];           //Allocate memory to array
    for(int i=0;i<N;i++) dl[i] = 0; //Clear array
    idx = 0;                        // Zero the array index
}

//-----------------------------------------------------

//Shift new data into delay line.  Call this a the sample rate.
//Returns data from delay line end  (max delay).

int32_t Delayline::iterate(int32_t in)
{
    
    int i = idx+1;          //Point to output data
    if(i >= length) i = 0;  //Check and correct for wrap-around.
    int32_t out = dl[i];    //read the data
    
    dl[idx] = in;                   // insert data
    if(++idx >= length) idx = 0;    //advance index and wrap-around if required.
    
    return out;
}

//---------------------------------------------------

//Enter with delay line regester number 0..N
//Returns delayed data from register N.


int32_t Delayline::getTap(int32_t N)
{
    if(N < 0) N = 0;                //Range checks
    if(N >= length) N = length-1;
    
    int32_t i = idx -1 - N ;     // i points to delay line register
    if(i < 0) i = length + i;    //Correct for ring buffer wrap-around
    
    return dl[i];                   //Return data
    

}

//-------------------------------------------------------