/** 
 * File holding functions need for the exoskeleton device. 
 *
 * 
 */ 

#include "functions.h"


bool perturbationDevice(MX28 exoMotor, float byAngle,  String &msg){


}

void printDouble( double val, unsigned int precision){
// prints val with number of decimal places determine by precision
// NOTE: precision is 1 followed by the number of zeros for the desired number of decimial places
// example: printDouble( 3.1415, 100); // prints 3.14 (two decimal places)

   Serial.print (int(val));  //prints the int part
   Serial.print("."); // print the decimal point
   unsigned int frac;
   if(val >= 0)
       frac = (val - int(val)) * precision;
   else
       frac = (int(val)- val ) * precision;
   Serial.println(frac,DEC) ;
} 

// Toggle board pin x number of times.
// @param boardPin
// @param tglnr, number of times to toggle.
// @param tglSpeed, delay between each state change
void toggleLEDXTimes(MX28 exoMotor, short boardPin, short tglnr, int tglSpeed)
{
  for (int i = 0; i < tglnr; i++)
  {
    delay(tglSpeed);
    digitalWrite(boardPin, LOW);
    exoMotor.setMotorLED(true);
    delay(tglSpeed);
    digitalWrite(boardPin, HIGH);
    exoMotor.setMotorLED(false);
  }
}


bool isWithinRange(float value, float target, float percentage){
  bool success = false;
  if(target*(float)(1-(percentage/100)) < value && value < target*(float)(1+(percentage/100))){
    success = true;
  }
  return success;
}

// bool isWithinRange(int32_t value, float target, float percentage){
//   bool success = false;
//   if(target * (float)(1-(percentage/100)) < value && value < target * (float)(1+(percentage/100))){
//     success = true;
//   }
//   return success;
// }