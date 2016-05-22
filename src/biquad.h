/*
 *  biquad.h
 *  
 *  Arduino 2nd order biquad filter library
 *  Created by Dale Heatherington on Jan 24, 2016.
 *  This Code is licensed under a Creative Commons Attribution-ShareAlike 3.0 Unported License.
 *  This source code is provided as is, without warranty.
 *  You may copy and distribute verbatim copies of this document.
 *  You may modify and use this source code to create binary code
 *  for your own purposes, free or commercial.
 *
 */

#ifndef biquad_h
#define biquad_h

/* Fixed point math macros from ARM application note 33 */

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




class Biquad

{
  
  public:

  enum filterType { LOWPASS,HIGHPASS,BANDPASS,NOTCH};
  
  
  Biquad(Biquad::filterType, double, double, double);       // constructor.
  void computeCoefficients(Biquad::filterType, double, double, double);
  int32_t iterate(int32_t);      //Do the biquad direct type 1 function. Returns filtered sample
   
    uint32_t samplePeriod();      //Return sample rate fs period in microseconds
  void print();
    
  
  private:
    filterType typef;
    const int32_t q = 14;   // q for fixed point math is 14
    double Qf;
    double aa0,aa1,aa2,bb1,bb2;
    int32_t a0,a1,a2,b0,b1,b2,x1,x2,y1,y2;
    uint32_t fsp;   //Sample rate period in microseconds = 1000000/sample rate
   

};

#endif

