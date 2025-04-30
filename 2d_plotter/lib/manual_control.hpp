#ifndef MANUAL_CONTROL_HPP
#define MANUAL_CONTROL_HPP

#include "motor_control.hpp"
#include "pen_control.hpp"

void setupManualControl();
void manualControlLoop(); // Đọc joystick và di chuyển
void toggleManualMode();  // Đổi chế độ Manual/Auto

#endif