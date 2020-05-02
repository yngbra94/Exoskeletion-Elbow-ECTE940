# 1 "c:\\Users\\Yngve Brathaug\\OneDrive - University of Wollongong\\Documents\\School OUW\\3 Third Semester\\ECTE940 Advanced Project\\Programming\\src\\ExoskeletonElbow.ino"
/*******************************************************************************

* Copyright 2016 ROBOTIS CO., LTD.

*

* Licensed under the Apache License, Version 2.0 (the "License");

* you may not use this file except in compliance with the License.

* You may obtain a copy of the License at

*

*     http://www.apache.org/licenses/LICENSE-2.0

*

* Unless required by applicable law or agreed to in writing, software

* distributed under the License is distributed on an "AS IS" BASIS,

* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

* See the License for the specific language governing permissions and

* limitations under the License.

*******************************************************************************/
# 17 "c:\\Users\\Yngve Brathaug\\OneDrive - University of Wollongong\\Documents\\School OUW\\3 Third Semester\\ECTE940 Advanced Project\\Programming\\src\\ExoskeletonElbow.ino"
/* Authors: Taehun Lim (Darby) */

# 20 "c:\\Users\\Yngve Brathaug\\OneDrive - University of Wollongong\\Documents\\School OUW\\3 Third Semester\\ECTE940 Advanced Project\\Programming\\src\\ExoskeletonElbow.ino" 2
# 21 "c:\\Users\\Yngve Brathaug\\OneDrive - University of Wollongong\\Documents\\School OUW\\3 Third Semester\\ECTE940 Advanced Project\\Programming\\src\\ExoskeletonElbow.ino" 2







// DXL serial baudrate. 


// Arduino serial baudrate






DynamixelWorkbench dxl_wb;

MX28Motor dxl_motor(1);

void setup()
{
  Serial.begin(57600);
  while(!Serial); // Wait for Opening Serial Monitor

  pinMode(14, 0x1); // Set LED pin 
  digitalWrite(14, 0x1); // HIGH is off ??

  const char *log;
  bool result = false;

  uint8_t dxl_id = 1;
  uint16_t model_number = 0;


  // Initialize the controller
  result = dxl_wb.init("1" /*Dynamixel on Serial3(USART3)*/, 1000000, &log);
  if (result == false)
  {
    Serial.println(log);
    Serial.println("Failed to init");
  }
  else
  {
    Serial.print("Succeeded to init : ");
    Serial.println(1000000);
  }

  // Ping the motor to check if it is responding. 
  result = dxl_wb.ping(dxl_id, &model_number, &log);
  if (result == false)
  {
    Serial.println(log);
    Serial.println("Failed to ping");
  }
  else
  {
    toggleLEDXTimes(14, 5, 250);
    Serial.println("Succeeded to ping");
    Serial.print("id : ");
    Serial.print(dxl_id);
    Serial.print(" model_number : ");
    Serial.println(model_number);
  }




}

int incomingByte = 0;

void loop()
{
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, 10);
  }
}





// Toggle board pin x number of times. 
// @param boardPin
// @param tglnr, number of times to toggle. 
// @param tglSpeed, delay between each state change
void toggleLEDXTimes(short boardPin, short tglnr, int tglSpeed){

     for(int i = 0; i < tglnr; i++){
      HAL_Delay(tglSpeed);
      digitalWrite(boardPin, 0x0);
      HAL_Delay(tglSpeed);
      digitalWrite(boardPin, 0x1);
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
