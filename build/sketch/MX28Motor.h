/* 
 *  MX28Motor.h - Library for controlling a Dynamixel MX 28 motor using
 *  the DynamixelWorkbech library. 
 *  
 *  Created by Yngve Brathaug May 2020. 
 */


#ifndef MX28Motor_h
#define MX28Motor_h

#include <DynamixelWorkbench.h>

              

class MX28Motor{
  public: 
    MX28Motor(short dxl_id);
    short getMotorID(); 
//    // void setAngle(int32_t angle); 
//    // void setAngleBoudries(int16_t minAngle, int16_t maxAngle); 
//    // void setMotorSpeed(int32_t motorSpeed);
//    // int32_t getCurrentAngle();
//
  private: 
   short _motorID; 

   // Constants
   
};

#endif
