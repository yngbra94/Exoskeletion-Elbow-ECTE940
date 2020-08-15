
/* Authors: Yngve Brathaug */

#include "MX28.h"

const uint8_t MX28_ID = 1;
// Arduino serial baudrate
#define BAUDRATE 57600
#define BOARD_LED_PIN 14
#define BOARD_BUTTON_PIN 23

// Debug flag
#define DEBUG_FLAG 1

// Creating object MX28 motor.
MX28 exoMotor(MX28_ID);

enum State
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

State state = CALIBARTION;

void setup()
{
  Serial.begin(BAUDRATE);
  while (!Serial);                                // Wait for Opening Serial Monitor
  pinMode(BOARD_LED_PIN, OUTPUT);    // Set LED pin
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
    toggleLEDXTimes(BOARD_LED_PIN, 5, 20);
    exoMotor.setTorqueOnOff(false);
  }
}

void loop()
{

  bool result = false;
  switch (state)
  {
  case INIT:
    break;

  case CALIBARTION:
    exoMotor.setTorqueOnOff(true);
    // exoMotor.setWheelMode();
    Serial.print("The pressent angle is: ");
    Serial.println(exoMotor.getPresentAngle());

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
