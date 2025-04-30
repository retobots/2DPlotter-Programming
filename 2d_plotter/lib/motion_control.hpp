#ifndef MOTION_CONTROL_HPP
#define MOTION_CONTROL_HPP

#include "motor_control.hpp"

void moveLine(float x1, float y1);
void moveArc(float cx, float cy, float radius, float startAngle, float endAngle);
void moveToSafeZ();

#endif