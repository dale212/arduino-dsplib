
//Differentiator example 

#include "differentiator.h"

//Note this differentiator has a gain of 1.62 and 
//usable frequency range of 0.17 times the sampling frequency.
//More info on this differentiator here:
//https://www.dsprelated.com/showarticle/35.php


Differentiator diff = Differentiator();  //No parameters

//Returns type int32_t

int32_t dval;


int32_t data[] = {0,0,0,0,0,0,100,100,100,100,100,100,100,100,0,0,0,0,0,0,100,200,300,400,500,600,700,800,900,1000,1100,1200} ;

int len = sizeof(data) /sizeof(data[0]);

void setup() {
  Serial.begin(9600);
  delay(1000);

}

void loop() {
  int i;
  
  for(i=0;i<len;i++){
    dval = diff.iterate(data[i]);
    Serial.print(data[i]);  //Print input
    Serial.print("  ");
    Serial.println(dval);   //Print output
    delay(250);
  } 


  while(1 == 1) ;   //Halt here after running once.

}
