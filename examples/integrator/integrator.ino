
// Integrator example code

#include "integrator.h"

//create instance of Integrator
//Parameters
// 1 low limit
// 2 high limit
// Parameters are type int32_t
// Data input and return values are int32_t

Integrator integ = Integrator(0,20); //Low limit 0, high limit 20

int i;
int32_t ival;

void setup() {
  Serial.begin(9600);
  delay(500);

  integ.reset();      //Reset integrator

}

void loop() {
  int data = 2 ;        //data input to integrator is +2
  
  for(i=0;i<20;i++){    //Iterate 20 times and print integrator value
    ival = integ.iterate(data);
    Serial.print(data);   //print input values
    Serial.print("  ");
    Serial.println(ival); //Print output values
    delay(100);
    
  }

  data = -1;              //Change data input from +2 to -1
  for(i=0;i<20;i++){      //Iterate 20 times
    ival = integ.iterate(data);
    Serial.print(data);     //print input values
    Serial.print("  ");
    Serial.println(ival);   //Print output values
    delay(100);
  }

  integ.reset();

  

}
