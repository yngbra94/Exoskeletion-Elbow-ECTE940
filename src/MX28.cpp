/*


*/

#include "MX28.h"
#include <DynamixelWorkbench.h>
                              

MX28::MX28(uint8_t dxl_id)
{
  motorID = dxl_id;
}

uint8_t MX28::getMotorID(){
  return motorID; 
}
