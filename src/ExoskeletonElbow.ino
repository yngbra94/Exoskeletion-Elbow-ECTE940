
/* Authors: Yngve Brathaug */

#include "MX28.h"
#include "functions.h"
#include "exoEnum.h"


const uint8_t MX28_ID = 1;
// Arduino serial baudrate
#define BAUDRATE_ARDUINO 57600

// Define input/output pins. 
#define BOARD_LED_PIN 14
#define BOARD_BUTTON_PIN 23

// Flags
bool exo_is_calibrated = false; 

// Debug flag
#define DEBUG_FLAG 1

// Creating object MX28 motor.
const float gearRatio = 0.5;
MX28 exoMotor(MX28_ID, gearRatio);
MotorParameters motorParameters;
SensorData mx28SensorData;


String msg;
unsigned long sensorUpdateTime = 0;
int sensorUpdataDelay = 1000; // ms

struct ExoskeletonParams{
  int perturbMode = 0; // 0: FWD->BWD, 1: BWD->FWD
  bool active_device = true;
  float angleToMove = 20;
  float perturbAtAngle = 50;
  float degThreshold = 2; //
  float velocityThreshold = 0; // Given in %
  float averageVelocity = 200;// 
}exoskeletonParams;




// Set initial switch states. 
Opperation_State operation_state = INIT;
Calibration_State calib_state = SETMIN;
Control_State contol_state = FREE_WEELING;
Perturbe_State perturb_state = INIT_PERT;

float startAngle = 0;
float endAngle = 0;



void setup()
{
  Serial.begin(BAUDRATE_ARDUINO);

  // while (!Serial){
  //   Serial.println("Waiting for seiral");
  // }             // Wait for Opening Serial Monitor
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
    toggleLEDXTimes(exoMotor, BOARD_LED_PIN, 5, 200);
    exoMotor.setTorqueOnOff(false);
  }

  // Initialise motor parameters. 
  success = exoMotor.initMotor();
}





void loop()
{

  // Success flag. 
  bool success = false;
  if(!exoskeletonParams.active_device){
    operation_state = TERMINATE;
  }


  switch (operation_state)
  {
  case INIT:
  // Init is currently done in the arduino setup.

    operation_state = CALIBARTION;
    Serial.println("*Calibration, set min");
    break; 

  case CALIBARTION: 
    // Disable torcque mode. 
    exoMotor.setTorqueOnOff(false);
    switch (calib_state){
      case SETMIN: 
        
        // Min position is max flexion
        if(!digitalRead(BOARD_BUTTON_PIN)){
          bool setMinSuccess = exoMotor.setMinAngle(mx28SensorData.minAngle);
          delay(500); // Wait for btn release
          if(setMinSuccess){
            toggleLEDXTimes(exoMotor , BOARD_LED_PIN, 2, 50);
            Serial.print("Min angle is set to ");
            Serial.println(mx28SensorData.minAngle);
            Serial.println("*Calibration, set max");
            calib_state = SETMAX;
          } else {
            Serial.print("Calib Err, could not set MIN position: ");
            Serial.println(exoMotor.getPresentPosition());
          } 
        }
        break;
      
      case SETMAX:
      // Max position is at max extension. 
        if(!digitalRead(BOARD_BUTTON_PIN)){
          // Might not be needed.
          bool successSetMaxAngle = exoMotor.setMaxAngle(mx28SensorData.maxAngle);
          if (successSetMaxAngle){
            toggleLEDXTimes(exoMotor ,BOARD_LED_PIN, 2, 50);
            exo_is_calibrated = true;
            Serial.print("Max angle is set to: ");
            Serial.println(mx28SensorData.maxAngle);
            Serial.println(exoMotor.getPresentPosition());
            Serial.println("Calibration, set max");
            Serial.println("*Free Wheeling");
            operation_state = ACTIVE_DEVICE;
          } else {
            Serial.println("Calib Err, could not set MAX position");
          }
        }
        break;

      default:
        Serial.println("[Error:] Calibration state does not exist"); 
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
      
      if(!digitalRead(BOARD_BUTTON_PIN)){
        Serial.println("[STATE:] PERTURBE");
        contol_state = PERTUBE;
      }

      if(exoMotor.getPresentAngle() == exoskeletonParams.perturbAtAngle){
        if(isWithinRange(exoMotor.getPresentVelocity(), exoskeletonParams.averageVelocity, exoskeletonParams.velocityThreshold)){
          //TODO, Use this to start pertubariont
          // Serial.println("[STATE:] PERTURBE");
          // contol_state = PERTUBE;
        }
      }
      break;

      case PERTUBE: 
        exoMotor.setTorqueOnOff(true);
        switch (perturb_state)
        {
          case INIT_PERT:
            // Set init profile
            startAngle = mx28SensorData.angle;
            // Check mode
            if(exoskeletonParams.perturbMode == 0){
              endAngle   = mx28SensorData.angle + exoskeletonParams.angleToMove;
            } else if(exoskeletonParams.perturbMode == 1){
              endAngle   = mx28SensorData.angle - exoskeletonParams.angleToMove;
            } 


            if(endAngle < mx28SensorData.minAngle){
              endAngle = mx28SensorData.minAngle;
            }
            else if (endAngle > mx28SensorData.maxAngle){
              endAngle = mx28SensorData.maxAngle;
            }

            perturb_state = PERT;
            Serial.println("Moving PERTURBE"); 
            break;
    
          case PERT:
            exoMotor.moveToAngle(endAngle);
            if(!isWithinRange(exoMotor.getPresentAngle(), endAngle, exoskeletonParams.degThreshold)){
              break;
            } else {
              perturb_state = HOME;
              Serial.println("Moving HOME");  
            }


          case HOME:
            exoMotor.moveToAngle(startAngle);
            if(isWithinRange(exoMotor.getPresentAngle(), startAngle, exoskeletonParams.degThreshold)){
              perturb_state = INIT_PERT;
              contol_state = FREE_WEELING;
              Serial.println("FREE_WEELING");
            } 
            break;

          default:
            // Fancy error Message 
            break;
        }

    
    default:
      break;
    }

  break;


  case TERMINATE:
    Serial.println("[Error]: Gerneral Error Message. \nProgram treminated.");
    // Add shut down. 

  default:
    break;
  }


    // Update sensor data. 
    if(!exoMotor.updateSensorData(mx28SensorData, msg)){
      Serial.println(msg);
    }
    if(DEBUG_FLAG > 2){
      if(millis() >= sensorUpdateTime + sensorUpdataDelay){
        sensorUpdateTime += sensorUpdataDelay;
        Serial.println("Sensor information are:");
        Serial.print("Angle: ");
        Serial.println(mx28SensorData.angle);
        Serial.print("Velocity: ");
        Serial.println(mx28SensorData.velocity);
        Serial.print("Load: ");
        Serial.println(mx28SensorData.load);
        Serial.print("Temp: ");
        Serial.println(mx28SensorData.temp);
      }
    }


    // Update parameters


    // TODO: make dirty flag
    if(!exoMotor.updateParamters(motorParameters, msg)){
      // Serial.println("[Err:] Could not update parametrs"); 
      // Serial.println(msg);
    }
}




