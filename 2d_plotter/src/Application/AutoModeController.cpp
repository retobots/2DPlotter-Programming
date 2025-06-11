#include "Application/AutoModeController.h"

AutoModeController::AutoModeController() {
  // Constructor
}

AutoModeController& AutoModeController::getInstance() {
  static AutoModeController instance;
  return instance;
}

void AutoModeController::setup() {
  // Cài đặt SDCard, LCD, parser...
}

void AutoModeController::run() {
  // Đọc file G-code → xử lý lệnh → điều khiển motor và servo
}
