//
//  oscillator.hpp
//  
//
//  Created by Dale Heatherington on 2/10/16.
//
//

#ifndef oscillator_h
#define oscillator_h



class Oscillator
{
public:
    Oscillator(double,double, double);      //Constructor (frequency,amplitude,sampling rate)
    
    void setFreq(double);                //Set frequency in hz
    void setFreqQ14(int32_t);            //Set frequency directly as fo/fs in Q14 fixed point format
    int32_t getFreqQ14();                //Get current frequency in Q14 fixed point format
    
    void setAmp(double);                //Set peak amplitude
    int32_t getAmpQ14(void);            //Get peak amplitude in Q14 format
    
    void    iterate(void);          //iterate to create new data point
    int32_t getSin(void);           // get Sine wave data point
    int32_t getCos(void);           // get cosine wave data point


    int32_t       f,a,sinZ,cosZ, sampleRate;
    const int32_t     q = 14;       //q for fixed point math is Q14
    
};



#endif /* oscillator_h */
