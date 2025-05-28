#pragma once
#include <Arduino.h>
class ManualModeController {
public:
  static ManualModeController& getInstance();

  void setup();
  void run();

private:
  ManualModeController();
  // Thêm biến trạng thái nếu cần
};
