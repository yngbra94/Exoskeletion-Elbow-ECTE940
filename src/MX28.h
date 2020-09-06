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


struct MotorParameters{
  int32_t maxVelocity     = 1022;
  int32_t maxAcceleration = 32750;

  int32_t PgainVelocity   = 16000;
  int32_t IgainVelocity   = 1920;
  
  int32_t PgainPosition   = 16000;
  int32_t IgainPosition   = 0;
  int32_t DgainPosition   = 0;
};


// Collection of sensor information. ¨
// NOTE: TO BE MOVED TO MX28.cpp
struct SensorData{
  float angle; 
  int32_t position; // Given in value. 
  int32_t minAngle;
  int32_t maxAngle;
  int32_t velocity;
  int32_t load;
  int32_t PWM;
  int32_t temp;
  int32_t inputVoltage;

  // Add sensor info. 
}; 

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
   bool initMotor();

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
   int32_t getPresentVelocity();
   //float getPressentAcceleration();

   bool updateParamters(MotorParameters motorParameters, String &msg);
   bool updateSensorData(SensorData& sensorData, String &msg);
   
   bool moveToAngle(float angle);
   bool moveToPosition(int32_t position);
   bool motorInMotion();
   bool goalReached();
   
   void setMotorLED(bool state);
   int32_t readItem(const char *item_name);

private:
  bool setOperationMode(uint8_t mode, const char **log = NULL);
  int32_t convertAngleToValue(const float &angle);
  float convertValueToAngle(const int32_t &angle_val);

  bool setVelocity(int32_t velocity);

  // Constant.
  uint8_t motor_ID;
  float gearRatio;
  float goalAngle;

};

#endif
