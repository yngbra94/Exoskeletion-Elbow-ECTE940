/*
@author Yngve Brathaug. 

This


*/

#include "MX28.h"

#define DEVICE_NAME "1" //Dynamixel on Serial3(USART3)
#define DXL_BAUDRATE 1000000

DynamixelWorkbench MotorMX28;



MX28::MX28(uint8_t dxl_id, float ratio)
{
     // Set motor ID n
     motor_ID = dxl_id;
     gearRatio = ratio; 
     
}
/**
    * Initialise the Dynamixel serial bus for the contoller and motor.
    * @param **log, Log message with infomation about the finised proccess. 
    * @return True if bus was succesfully Initialised and MX28 was found, False if not
    * */
bool MX28::initBus(const char **log)
{
     bool result = false;
     result = MotorMX28.init(DEVICE_NAME, DXL_BAUDRATE, log);
     if (result == false)
     {
          return result;
     }
     else
     {
          result = MotorMX28.ping(motor_ID);
          if (result)
          {
               return result;
          }
          else
          {
               result = false;
               return result;
          }
     }
}

uint8_t MX28::getMotorID()
{
     return motor_ID;
}

/**
     * Writes the motor ID to the motor. 
     * @param motorID The motor ID to write. 
     * */
bool MX28::setMotorID(uint8_t motorID)
{
     uint8_t current_ID = getMotorID();
     bool result = false;
     // If the current motor is the same as the new, return current ID
     if (current_ID == motorID)
     {
          return result;
     }
     // If not, Change to the new adress.
     else
     {
          result = MotorMX28.changeID(current_ID, motorID);
     }
     return result;
}

/**
 * Set up the motor config. 
 */
bool MX28::initMotor(){
     bool success = false; 

     // Reverse the motor direction. 
     success = setTorqueOnOff(false);
     success = MotorMX28.itemWrite(motor_ID, "Drive_Mode", 0x01);
     success = MotorMX28.itemWrite(motor_ID, "Operating_Mode", 0x04);
     success = MotorMX28.itemWrite(motor_ID, "Homing_Offset", 0); // Reset to initial value
     success = MotorMX28.itemWrite(motor_ID, "Min_Position_Limit", 0);
     success = MotorMX28.itemWrite(motor_ID, "Max_Position_Limit", 1);
     return success;
}

/**
     * Sets the motor in Wheeling mode. This makes it free to move.
     * @returns True if successful, False if not. 
     * */
bool MX28::setWheelMode(int32_t acceleration)
{
     return MotorMX28.wheelMode(motor_ID, acceleration);
}

/**
     * Sets the motor in Joint mode. This makes the motor ready to be controlled.
     * @returns True if successful, False if not. 
     * */
bool MX28::setJointMode(int32_t velocity, int32_t acceleration)
{
     return MotorMX28.jointMode(motor_ID, velocity, acceleration);
}


/** 
 * Set motor in torque control 
 * */
bool MX28::setTorqueMode()
{
    return MotorMX28.setTorqueControlMode(motor_ID);
}


/**
   * Turn on or of the motor torque
   * @param noOff, Set tru for torque on, False for off
   * @returns True if successful, False if not. 
   * */
bool MX28::setTorqueOnOff(bool onOff, const char **log)
{
     if (onOff)
     {
          return MotorMX28.torqueOn(motor_ID, log);
     }
     else
     {
          return MotorMX28.torqueOff(motor_ID, log);
     }
}

/**
     * Set the minium angle the motor is allowed to move to. 
     * @param angle The minium allowed angle
     * @returns True if successful, False if not.
     * */
bool MX28::setMinAngle(int32_t &minAngle)
{    
     bool success = false;
     // Update min value
     
     int32_t currentPos = getPresentPosition();

     success = MotorMX28.itemWrite(motor_ID, "Homing_Offset", currentPos);

     if(success){
          success = MotorMX28.itemWrite(motor_ID, "Min_Position_Limit", currentPos);
     }
     minAngle = getPresentAngle();
     return success;

}

     /**
     * Set the maximum angle the motor is allowed to move to. 
     * @param angle The maximum allowed angle
     * @returns True if successful, False if not.
     **/
bool MX28::setMaxAngle(int32_t &maxAngle)
{
     maxAngle = getPresentAngle();
     // check if min angle are larger than max angle. 
     int32_t minAngle;
     MotorMX28.itemRead(motor_ID, "Min_Position_Limit", &minAngle);
     // if(maxAngle < minAngle){
     //      // Write fincy error msg. 
     //      return false;
     // }
     return MotorMX28.itemWrite(motor_ID, "Max_Position_Limit", maxAngle);
}
// Set max angel as pressent position.

/** 
     * Reads the present angle from the motor. 
     * @return The present angle of the motor given in degrees. 
* */
float MX28::getPresentAngle()
{
     int32_t analog_angle;
     MotorMX28.getPresentPositionData(motor_ID, &analog_angle);
     return convertValueToAngle(analog_angle);
}

/** 
     * Reads the present position from the motor. 
     * @return The present position in value
     * */
int32_t MX28::getPresentPosition()
{
     int32_t present_position;
     MotorMX28.getPresentPositionData(motor_ID, &present_position);
     return present_position;

}

 /**
  * Return present velocity
  */
int32_t MX28::getPresentVelocity(){
     int32_t present_velocity;
     MotorMX28.itemRead(motor_ID, "Present_Velocity", &present_velocity);
     return present_velocity;
}

 /**
  * Set velocity
  */
bool MX28::setVelocity(const int32_t velocity){
     return MotorMX28.itemWrite(motor_ID, "Velocity_Limit", velocity);
}

/**
 * Udate all parameters in the MotorParamtes struct (MX28.h)
 */
bool MX28::updateParamters(MotorParameters motorParameters, String &msg){
     bool success = true;
     msg = "";
     if(!MotorMX28.itemWrite(motor_ID, "Velocity_Limit", motorParameters.maxVelocity)){
          msg += "[Error:] Could not update Velocity_Limit \n";
          success = false;
     }
     if(!MotorMX28.itemWrite(motor_ID, "Acceleration_Limit", motorParameters.maxAcceleration)){
          msg += "[Error:] Could not update Acceleration_Limit \n";
          success = false;
     }
     if(!MotorMX28.itemWrite(motor_ID, "Velocity_P_Gain", motorParameters.PgainVelocity)){
          msg += "[Error:] Could not update Velocity_P_Gain\n";
          success = false;
     }
     if(!MotorMX28.itemWrite(motor_ID, "Velocity_I_Gain", motorParameters.IgainVelocity)){
          msg += "[Error:] Could not update Velocity_I_Gain\n";
          success = false;
     }
     if(!MotorMX28.itemWrite(motor_ID, "Position_P_Gain", motorParameters.PgainPosition)){
          msg += "[Error:] Could not update Position_P_Gain\n";
          success = false;
     }
     if(!MotorMX28.itemWrite(motor_ID, "Position_I_Gain", motorParameters.IgainPosition)){
          msg += "[Error:] Could not update Position_I_Gain\n";
          success = false;
     }
     if(!MotorMX28.itemWrite(motor_ID, "Position_D_Gain", motorParameters.DgainPosition)){
          msg += "[Error:] Could not update Position_D_Gain\n";
          success = false;
     }
     if(success){
          msg = "Motor parameters uppdated.";
     }
     return success;
}


bool MX28::updateSensorData(SensorData& sensorData, String &msg){
     bool success = true;
     msg = "";
     sensorData.angle = getPresentAngle();

     if(!MotorMX28.itemRead(motor_ID, "Present_Position", &sensorData.position)){
          msg += "[Error:] Could not get Present_Position\n";
          success = false;  
     }
     if(!MotorMX28.itemRead(motor_ID, "Present_Velocity", &sensorData.velocity)){
          msg += "[Error:] Could not get Present_Velocity\n";
          success = false;  
     }
     if(!MotorMX28.itemRead(motor_ID, "Present_Load", &sensorData.load)){
          msg += "[Error:] Could not get Present_Load\n";
          success = false;  
     }
     if(!MotorMX28.itemRead(motor_ID, "Present_PWM", &sensorData.PWM)){
          msg += "[Error:] Could not get Present_PWM\n";
          success = false;  
     }
     if(!MotorMX28.itemRead(motor_ID, "Present_Temperature", &sensorData.temp)){
          msg += "[Error:] Could not get Present_Temperature\n";
          success = false;  
     }
     if(!MotorMX28.itemRead(motor_ID, "Present_Input_Voltage", &sensorData.inputVoltage)){
          msg += "[Error:] Could not get Present_Input_Voltage\n";
          success = false;  
     }
     if(success){
          msg = "Sensor data updated";
     }
     return success;
}



 /** 
 * Mover the motor to given angle. 
 * @param angle Move to this angle.
 * */
bool MX28::moveToAngle(float angle)
{
     goalAngle = angle;
     int32_t pos = convertAngleToValue(angle);
     return MotorMX28.goalPosition(motor_ID, pos);
     
}


/** 
     * Mover the motor to given positon. 
     * @param position goal position. 
     * */
bool MX28::moveToPosition(int32_t position)
{
     return MotorMX28.goalPosition(motor_ID, position);
}

/**
     * Check it the robot is in motion. 
     * @return True if the motor is moving. 
     * */
bool MX28::motorInMotion()
{
     int32_t isMoving; 
     MotorMX28.itemRead(motor_ID, "Moving", &isMoving);
     return (bool)isMoving;
}

/**
     * Check if to motion goal is reached.  
     * @return True if the motor has reached the desired position. 
     * */
bool MX28::goalReached(){
     if(goalAngle == getPresentAngle()){
          return true;
     } else {
          return false;
     }
}

/**
    * set motor LED
    * @param state, True for on, False for off.
    * */
void MX28::setMotorLED(bool state)
{
     if (state)
     {
          MotorMX28.ledOn(motor_ID);
     }
     else
     {
          MotorMX28.ledOff(motor_ID);
     }
}

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
bool MX28::setOperationMode(uint8_t mode, const char **log)
{
     MotorMX28.setOperatingMode(motor_ID, mode, log);
}

/**
     * Convert an angle (0-360) to the  value readed from and to the motor. 
     * @param angle The angle to convert. 
     * @returns The value rotational value used by the motor. 
     * */
int32_t MX28::convertAngleToValue(const float &angle)
{
     return int32_t(angle*(4095.000/(360.000*gearRatio)));
}

/**
     * Convert an  value (0-4095) at an Angle (0-360)
     * 
     * @param analog_angle_val The analog angle value (0-4095). 
     * @returns The value rotational value used by the motor. 
     * */
float MX28::convertValueToAngle(const int32_t &angle_val) {
     float angle = gearRatio* angle_val * float(360.000/ 4095.000);
     return angle;
} 



 int32_t MX28::readItem(const char *item_name){
      
      int32_t result = 0; 
      MotorMX28.itemRead(motor_ID, item_name, &result);
      return result;
 }
