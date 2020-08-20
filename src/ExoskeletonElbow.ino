
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
bool active_device = true;

// Debug flag
#define DEBUG_FLAG 1

// Creating object MX28 motor.
const float gearRatio = 0.5;
MX28 exoMotor(MX28_ID, gearRatio);


// Collection of sensor information. ¨
// NOTE: TO BE MOVED TO MX28.cpp
struct sernorData{
  int32_t angle; 
  int32_t position; // Given in value. 
  int32_t minAngle;
  int32_t maxAngle;
  int32_t velocity;

  // Add sensor info. 
} mx28SensorData; 



enum Opperation_State
{
  INIT,
  CALIBARTION,
  ACTIVE_DEVICE,
  TERMINATE
};

enum Calibration_State{
  SETMIN, 
  SETMAX
};

enum Control_State{
  FREE_WEELING, 
  PERTUBE
};
// Set initial switch states. 
Opperation_State opperation_state = CALIBARTION;
Calibration_State calib_state = SETMIN;
Control_State contol_state = FREE_WEELING;

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

  // 
  if(!active_device){
    opperation_state = TERMINATE;
  }


  switch (opperation_state)
  {
  case INIT:
  // Init is currently done in the arduino setup.
    break; 

  case CALIBARTION:
    // Disable torcque mode. 
    exoMotor.setTorqueOnOff(false);
    switch (calib_state){
      case SETMIN: 
        // Min position is max flexion
        if(!digitalRead(BOARD_BUTTON_PIN)){
          bool setMinSuccess = exoMotor.setMinAngle(mx28SensorData.minAngle);
          if(setMinSuccess){
            toggleLEDXTimes(BOARD_LED_PIN, 2, 50);
            Serial.println("Min angle is set");
            calib_state = SETMAX;
          } 
        }
        break;
      
      case SETMAX:
      // Max position is at max extension. 
        if(!digitalRead(BOARD_BUTTON_PIN)){
          // Might not be needed.
          bool successSetMaxAngle = exoMotor.setMaxAngle(mx28SensorData.maxAngle);
          if (successSetMaxAngle){
            toggleLEDXTimes(BOARD_LED_PIN, 2, 50);
            exo_is_calibrated = true;
            Serial.println("Max angle is set");
            opperation_state = ACTIVE_DEVICE;
          } else {
            // Add error message. 
            break; 
          }
        }
        break;

      default:
        Serial.println("Err: Calibration state does not exist"); 
        break;  
    }
    break;

  case ACTIVE_DEVICE:
    // Main case.
    switch (contol_state)
    {
    case FREE_WEELING:
      // In this mode the are should be able to move freely with minimum restrictions. 
      exoMotor.setTorqueOnOff(false);
      Serial.println("Free Wheeling");
      if(!digitalRead(BOARD_BUTTON_PIN)){
        contol_state = PERTUBE;
      }
      break;

    case PERTUBE: 
      // Read current position. 
      Serial.println("Perturbe");
      mx28SensorData.position = exoMotor.getPresentPosition();
      mx28SensorData.angle = exoMotor.getPresentAngle();
      // TODO: Make different perturbation modes. 
      // Perturbe the arm 10 degrees extension and return 
      Serial.print("The arm is moving from ");
      Serial.print(mx28SensorData.angle); 
      Serial.print(" to ");
      Serial.print(mx28SensorData.angle + 5); 

      // Return to free wheeling mode. 


      break;
    
    default:
      break;
    }

    
    // exoMotor.setTorqueOnOff(true);
    // Serial.println(exoMotor.setJointMode(100, 10000));

    mx28SensorData.angle = exoMotor.getPresentAngle();
    // if(DEBUG_FLAG > 0){
    //   Serial.println("Sensor information are:");
    //   Serial.print("Angle: ");
    //   Serial.println(mx28SensorData.angle);
    //   Serial.print("Min angle: ");
    //   Serial.println(mx28SensorData.minAngle);
    //   Serial.print("Max angle: ");
    //   Serial.println(mx28SensorData.maxAngle);
    // }
  break;


  case TERMINATE:
    // If error occure or device is swiched of, temrinate.  
    break;

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
