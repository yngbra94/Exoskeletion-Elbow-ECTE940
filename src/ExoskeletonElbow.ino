
/* Authors: Yngve Brathaug */

#include <DynamixelWorkbench.h>

#define DEVICE_NAME "1" //Dynamixel on Serial3(USART3)


// DXL serial baudrate. 
#define DXL_BAUDRATE  1000000
const uint8_t MX28_ID = 1;
// Arduino serial baudrate
#define BAUDRATE 57600
#define BOARD_LED_PIN 14

// Debug flag
#define DEBUG_FLAG 1 

// Creating object MX28 motor. 
DynamixelWorkbench MX28;

enum State {INIT, CALIBARTION, TORQUE_CONTROL, MOVE_HOME, MOVE_TO}; 
State state = TORQUE_CONTROL; 



void setup() 
{
  Serial.begin(BAUDRATE);
  while(!Serial); // Wait for Opening Serial Monitor

  pinMode(BOARD_LED_PIN, OUTPUT); // Set LED pin 
  digitalWrite(BOARD_LED_PIN, HIGH); // HIGH is off ??
  
 
  bool result = false;
  uint16_t model_number = 0;


  // Initialize the controller for motor
  result = MX28.init(DEVICE_NAME, DXL_BAUDRATE);
  if (result == false)
  {
    Serial.println("Failed to init");
  }
  else
  {
    Serial.print("Succeeded to init : ");
    Serial.println(DXL_BAUDRATE);  
  }
  
  

  // Ping the motor to check if it is responding. 
  result = MX28.ping(MX28_ID, &model_number);
  if (result == false)
  {
    Serial.println("Failed to ping");
  }
  else
  {
    toggleLEDXTimes(BOARD_LED_PIN, 5, 250);
    Serial.println("Succeeded to ping");
    Serial.print("id : ");
    Serial.print(MX28_ID);
    Serial.print(" model_number : ");
    Serial.println(model_number);
  }

  Serial.println(MX28.getModelName(MX28_ID)); 

  
  
}



void loop() 
{




  bool result = false; 
  switch(state)
  {
    case INIT:
      break;

    case TORQUE_CONTROL: 

    result = MX28.setTorqueControlMode(MX28_ID);
    if (result){
      Serial.println("Motor in torque control mode");
    } else {
      Serial.println("Failed to set motor in torque control, "); 
     
    }
    state = INIT; 

    case MOVE_HOME: 
      MX28.goalPosition(MX28_ID, (int32_t)0);
      delay(200); 
      state = MOVE_HOME; 

    case MOVE_TO: 
      MX28.goalPosition(MX28_ID, (int32_t)500);
      delay(500); 
      state = INIT; 

    default:
      break;
  }

}








// Toggle board pin x number of times. 
// @param boardPin
// @param tglnr, number of times to toggle. 
// @param tglSpeed, delay between each state change
void toggleLEDXTimes(short boardPin, short tglnr, int tglSpeed){
     for(int i = 0; i < tglnr; i++){
      delay(tglSpeed);
      digitalWrite(boardPin, LOW);
      MX28.ledOn(MX28_ID); 
      delay(tglSpeed);
      digitalWrite(boardPin, HIGH);
      MX28.ledOff(MX28_ID); 
    }
}




//  result = dxl_wb.jointMode(dxl_id, 0, 0, &log);
//  if (result == false)
//  {
//    Serial.println(log);
//    Serial.println("Failed to change joint mode");
//  }
//  else
//  {
//    Serial.println("Succeed to change joint mode");
//    Serial.println("Dynamixel is moving...");
//
//    for (int count = 0; count < 5; count++)
//    {
//      dxl_wb.goalPosition(dxl_id, (int32_t)0);
//      digitalWrite(BOARD_LED_PIN, HIGH); 
//      delay(500);
//
//      dxl_wb.goalPosition(dxl_id, (int32_t)20);
//      digitalWrite(BOARD_LED_PIN, LOW); 
//      delay(500);
//    }
//  }
