/* 
 *  MX28Motor.h - Library for controlling a Dynamixel MX 28 motor using
 *  the DynamixelWorkbech library. 
 *  
 *  Created by Yngve Brathaug May 2020. 
 */

#ifndef MX28_h
#define MX28_h
/*
  The DynamixelWorkbench can be downloaded at: 
  http://emanual.robotis.com/docs/en/software/dynamixel/dynamixel_workbench/

  It is also installed by default when setting up the Arduino IDE with the OpenCM9.04 controller. 
  This can be found in point 8.3 in the OpenCM9.04 E-Manual: 
  http://emanual.robotis.com/docs/en/parts/controller/opencm904/#arduino-ide
*/

#include <DynamixelWorkbench.h>
#include "MX28_ControlTable.h"


class MX28
{
public:
  // public methods.
  /**
    * Constructor. 
    * Setts the motor ID.
    * @param dxl_id Motor id. 
    */
  MX28(uint8_t dxl_id, float gearRatio);

   bool initBus(const char **log = NULL);

   uint8_t getMotorID();
   bool setMotorID(uint8_t motorID);
   
   bool setWheelMode(int32_t acceleration = 0);
   bool setJointMode(int32_t velocity = 0, int32_t acceleratio = 0);
   bool setTorqueMode();
   bool setTorqueOnOff(bool onOff, const char **log = NULL);
   
   bool setMinAngle(int32_t &minAngle);
   bool setMaxAngle(int32_t &maxAngle);
   float getPresentAngle();
   int32_t getPresentPosition();
   
   bool moveToAngle(int16_t angle);
   bool moveToPosition(int32_t position);
   bool motorInMotion();
   bool goalReached();
   
   void setMotorLED(bool state);


   /**
   * 
   * 
   */
   int32_t readItem(const char *item_name);

private:
  /** 
     * Setts the opperation mode. 
     * @param mode desired opperation mode. 
     * @returns True if the mode is successfuly changed. 
     * 
     * Modes available: 
     * 1, Velocity Control Mode
     * 3, Position Control Mode (Dafault)
     * 4, Extended Position Control Mode. Multi turn mode. !! NOT IN USE IN THE EXOSKELETON PROJECT. 
     * 16, PWM Control Mode (Voltage Control Mode)
     * */
  bool setOpperationMode(uint8_t mode, const char **log = NULL);

  /**
     * Convert an angle (0-360) to the analog value readed from and to the motor. 
     * @param angle The angle to convert. 
     * @returns The value rotational value used by the motor. 
     * */
  int32_t convertAngleToValue(float angle);

  /**
     * Convert an Analog value (0-4095) at an Angle (0-360)
     * 
     * @param analog_angle_val The analog angle value (0-4095). 
     * @returns The value rotational value used by the motor. 
     * */
  float convertValueToAngle(int32_t analog_angle_val);




  

  // Constant.
  uint8_t motor_ID;
  float gearRatio;
};

#endif
