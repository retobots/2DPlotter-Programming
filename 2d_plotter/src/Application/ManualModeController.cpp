#include "ManualModeController.h"

ManualModeController::ManualModeController() {
  // Constructor
}

ManualModeController& ManualModeController::getInstance() {
  static ManualModeController instance;
  return instance;
}

void ManualModeController::setup() {
  // Cài đặt tay cầm PS4, LCD, motor...
}

void ManualModeController::run() {
  // Đọc joystick → xử lý điều khiển motor và servo
}
