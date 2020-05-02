/* 
 *  MX28Motor.h - Library for controlling a Dynamixel MX 28 motor using
 *  the DynamixelWorkbech library. 
 *  
 *  Created by Yngve Brathaug May 2020. 
 */


#ifndef MX28_h
#define MX28_h

#include <DynamixelWorkbench.h>

              

class MX28{
  public: 
    MX28(uint8_t dxl_id);

    uint8_t getMotorID(); 
    void setMotorID(uint8_t motorID); 
    
    void setAngle(int32_t angle); 
//    // void setAngleBoudries(int16_t minAngle, int16_t maxAngle); 
//    // void setMotorSpeed(int32_t motorSpeed);
//    // int32_t getCurrentAngle();
//
  private: 
   short motorID; 

   // Constants
   
};

#endif
