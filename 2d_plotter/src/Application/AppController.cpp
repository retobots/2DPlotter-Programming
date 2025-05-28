#include "AppController.h"
#include "AutoModeController.h"
#include "ManualModeController.h"

AppController::AppController() {
  // Constructor
}

AppController& AppController::getInstance() {
  static AppController instance;
  return instance;
}

void AppController::setup() {
  // Khởi tạo các thành phần cần thiết, hiển thị menu
}

void AppController::loop() {
  // Lắng nghe input menu và gọi controller tương ứng
  // Ví dụ:
  // if (manualModeSelected) ManualModeController::getInstance().run();
  // else if (autoModeSelected) AutoModeController::getInstance().run();
}
