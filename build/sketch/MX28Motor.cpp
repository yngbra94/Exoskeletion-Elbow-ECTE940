
#include "MX28Motor.h"
                              

MX28Motor::MX28Motor(short dxl_id)
{
  _motorID = dxl_id;
}

short MX28Motor::getMotorID(){
  return _motorID; 
}
