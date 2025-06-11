#include "HAL/PS4ControllerHAL.h"

PS4ControllerHAL::PS4ControllerHAL() {}

PS4ControllerHAL& PS4ControllerHAL::getInstance() {
  static PS4ControllerHAL instance;
  return instance;
}

void PS4ControllerHAL::setup() {
  // Kết nối Bluetooth với PS4
}

int PS4ControllerHAL::getAnalogX() {
  // Trả về giá trị analog trục X
  return 0;
}

int PS4ControllerHAL::getAnalogY() {
  // Trả về giá trị analog trục Y
  return 0;
}

bool PS4ControllerHAL::isButtonPressed() {
  // Trả về true nếu nút joystick được nhấn
  return false;
}
