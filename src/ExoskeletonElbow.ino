
/* Authors: Yngve Brathaug */

#include "MX28.h"


const uint8_t MX28_ID = 1;
// Arduino serial baudrate
#define BAUDRATE_ARDUINO 57600

// Define input/output pins. 
#define BOARD_LED_PIN 14
#define BOARD_BUTTON_PIN 23

// 
bool exo_is_calibrated = false; 

// Debug flag
#define DEBUG_FLAG 1

// Creating object MX28 motor.
const float gearRatio = 0.5;
MX28 exoMotor(MX28_ID, gearRatio);



enum Control_State
{
  INIT,
  CALIBARTION,
  WHEEL_MODE,
  JOINT_MODE,
  PERTURBATE,
  TORQUE_CONTROL,
  MOVE_HOME,
  MOVE_TO
};

enum Calibration_State{
  SETMIN, 
  SETMAX
};
Control_State control_state = CALIBARTION;
Calibration_State calib_state = SETMIN;

void setup()
{
  Serial.begin(BAUDRATE_ARDUINO);

  while (!Serial){
    Serial.println("Waiting for seiral");
  }             // Wait for Opening Serial Monitor
  pinMode(BOARD_LED_PIN, OUTPUT);    // Set LED pin
  pinMode(BOARD_BUTTON_PIN, INPUT_PULLUP);
  digitalWrite(BOARD_LED_PIN, HIGH); // HIGH is off ??

  bool success = false;
  uint16_t model_number = 0;
  const char *log;

  // Initialise the Dynamixel serial bus
  success = exoMotor.initBus(&log);
  if (success == false)
  {
    Serial.println(log);
    Serial.println("Did not find MX28 Motor");
  }
  else
  {
    Serial.println(log);
    Serial.println("Found MX 28 Motor");
    toggleLEDXTimes(BOARD_LED_PIN, 5, 200);
    exoMotor.setTorqueOnOff(false);
  }
}

void loop()
{

  bool result = false;
  switch (control_state)
  {
  case INIT:
  // Init is currently done in the arduino setup.
    break; 

  case CALIBARTION:
    // Disable torcque mode. 
    exoMotor.setTorqueOnOff(false);
    Serial.println(exoMotor.readItem("Punch"));
    // Let arm  move to min position.
    // Serial.println(digitalRead(BOARD_BUTTON_PIN));
    switch (calib_state){
      case SETMIN: 
        if(!digitalRead(BOARD_BUTTON_PIN)){
          Serial.println(exoMotor.setMinAngle(5));
          Serial.print("The Homing offset is: ");
          Serial.println(exoMotor.readItem("CW_Angle_Limit"));
          calib_state = SETMAX;
        }
      
      case SETMAX:
        



        exo_is_calibrated = true;
    }
    // Wait for button pressed, 
    // Set new min value. 
    // Let arm move to max position. 
    // Wait for button pressed. 
    // Set max value. 

    // Change to state 3, (update values from sensors etc)

  case TORQUE_CONTROL:

  case MOVE_HOME:

  case MOVE_TO:

  default:
    break;
  }
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
void toggleLEDXTimes(short boardPin, short tglnr, int tglSpeed)
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
