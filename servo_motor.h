/*
  driver for servo motor
  not sure which brand
*/

#ifndef _SERVO_MOTOR_H_
#define _SERVO_MOTOR_H_

#include "setup.h"

void init_servo(void);
void servo(float angle, float pitch);

#endif
