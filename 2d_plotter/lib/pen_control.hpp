#ifndef PEN_CONTROL_HPP
#define PEN_CONTROL_HPP

#include "config.hpp"
#include <Servo.h>
#include <Arduino.h>

void setupPen();
void penUp();
void penDown();
void togglePen(); // Đảo trạng thái bút

#endif