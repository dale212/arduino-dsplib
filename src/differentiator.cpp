//
//  differentiator.cpp
//  
//
//  Created by Dale Heatherington on 2/22/16.
//
//  Note this differentiator has a gain of 1.62 and
//  usable frequency range of 0.17 times the sampling frequency.
//  From an article by Rick Lyons at https://www.dsprelated.com/showarticle/35.php
//  Title:  A differentiator with a difference

#include "differentiator.h"


/* Fixed point math macros from ARM application note 33
 Only TOFIX() used here .
 */

/* convert a from q1 format to q2 format */
#define FCONV(a, q1, q2) (((q2)>(q1)) ? (a)<<((q2)-(q1)) : (a)>>((q1)-(q2)))


/* The basic operations perfomed on two numbers a and b of fixed
 point q format returning the answer in q format */
#define FADD(a,b) ((a)+(b))
#define FSUB(a,b) ((a)-(b))
#define FMUL(a,b,q) (((a)*(b))>>(q))
#define FDIV(a,b,q) (((a)<<(q))/(b))


/* The basic operations where a is of fixed point q format and b is
 an integer */
#define FADDI(a,b,q) ((a)+((b)<<(q)))
#define FSUBI(a,b,q) ((a)-((b)<<(q)))
#define FMULI(a,b) ((a)*(b))
#define FDIVI(a,b) ((a)/(b))

/* the general operation between a in q1 format and b in q2 format
 returning the result in q3 format */
#define FADDG(a,b,q1,q2,q3) (FCONV(a,q1,q3)+FCONV(b,q2,q3))
#define FSUBG(a,b,q1,q2,q3) (FCONV(a,q1,q3)-FCONV(b,q2,q3))
#define FMULG(a,b,q1,q2,q3) FCONV((a)*(b), (q1)+(q2), q3)
#define FDIVG(a,b,q1,q2,q3) (FCONV(a, q1, (q2)+(q3))/(b))

/* convert to and from floating point */
#define TOFIX(d, q) ((int)( (d)*(double)(1<<(q)) ))
#define TOFLT(a, q) ( (double)(a) / (double)(1<<(q)) )


Differentiator::Differentiator(void)
{
    z1 = z2 = z3 = z4 = z5 = z6 = 0;
}

//-------------------------------------------------

int32_t Differentiator::iterate(int32_t in)
{
    
    int32_t in_q = in << q ;       //Convert input to q14 format
    
    int32_t out = -(in_q >>4) + z2 -z4 + (z6 >>4);
    
    z6 = z5;
    z5 = z4;
    z4 = z3;
    z3 = z2;
    z2 = z1;
    z1 = in_q;
    
    return out >> q;        //Convert q format back to int.
    
}

//----------------------------------------------------

/* End */
