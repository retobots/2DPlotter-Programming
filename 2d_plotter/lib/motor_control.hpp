#ifndef MOTOR_CONTROL_HPP
#define MOTOR_CONTROL_HPP

#include "config.hpp"
#include "Arduino.h"
//Include Libraries

void setupMotors();
void moveMotorX(float distance_mm);
void moveMotorY(float distance_mm);
void moveMotorTo(float x_mm, float y_mm);
void homePosition();
void setMotorSpeed(int speedX, int speedY);

// Optionally: Hỗ trợ tạm dừng motor
void stopMotors();

#endif