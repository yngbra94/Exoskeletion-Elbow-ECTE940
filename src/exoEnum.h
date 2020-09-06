// Enums for ExoskeletonElbow.ino
#ifndef EXOENUM_H
#define EXOENUM_H
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

enum Perturbe_State{
  INIT_PERT,
  PERT,
  HOME
};



#endif