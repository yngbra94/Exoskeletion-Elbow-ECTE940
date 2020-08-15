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

class MX28
{
public:
  // public methods.
  /**
    * Constructor. 
    * Setts the motor ID.
    * @param dxl_id Motor id. 
    */
  MX28(uint8_t dxl_id);

  /**
    * Initialise the Dynamixel serial bus for the contoller and motor.
    * @param **log, Log message with infomation about the finised proccess. 
    * @return True if bus was succesfully Initialised
    * */
  bool initBus(const char **log = NULL);

  /**
     * Get motor id reads the ID from the motor and returns it. 
     * @returns The motor ID
     * */
  uint8_t getMotorID();

  /**
     * Writes the motor ID to the motor. 
     * @param motorID The motor ID to write.
     * @return True if ide was successfully changed.  
     * */
  bool setMotorID(uint8_t motorID);

  /**
     * Sets the motor in Wheeling mode. This makes it free to move.
     * @returns True if successful, False if not. 
     * */
  bool setWheelMode(int32_t acceleration = 0);

  /**
     * Sets the motor in Joint mode. This makes the motor ready to be controlled.
     * @returns True if successful, False if not. 
     * */
  bool setJointMode(int32_t velocity = 0, int32_t acceleratio = 0);

  /**
   * Turn on or of the motor torque
   * @param noOff, Set tru for torque on, False for off
   * */
  bool setTorqueOnOff(bool onOff, const char **log = NULL);

  /**
     * Set the minium angle the motor is allowed to move to. 
     * @param angle The minium allowed angle
     * @returns True if successful, False if not.
     * */
  bool setMinAngle(uint16_t angle);
  // Set Home position.
  // Set min angel as pressent position

  /**
     * Set the maximum angle the motor is allowed to move to. 
     * @param angle The maximum allowed angle
     * @returns True if successful, False if not.
     * */
  bool setMaxAngle(uint16_t angle);
  // Set max angel as pressent position.

  /** 
     * Reads the present angle from the motor. 
     * @return The present angle of the motor. 
     * */
  float getPresentAngle();

  /** 
     * Mover the motor to given angle. 
     * @param angle Move to this angle.
     * */

  void moveToAngle(uint16_t angle);

  /**
     * Check it the robot is in motion. 
     * @return True if the motor is moving. 
     * */
  bool motorInMotion();

  /**
     * Check if to motion goal is reached.  
     * @return True if the motor has reached the desired position. 
     * */
  bool goalReached();

  /**
    * set motor LED
    * @param state, True for on, False for off.
    * */
  void setMotorLED(bool state);

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
  int32_t convertAngleToAnalog(float angle);

  /**
     * Convert an Analog value (0-4095) at an Angle (0-360)
     * 
     * @param analog_angle_val The analog angle value (0-4095). 
     * @returns The value rotational value used by the motor. 
     * */
  float convertAnalogToAngle(int32_t analog_angle_val);

  // Constant.
  uint8_t motor_ID;
};

#endif
