#pragma once
#include <Arduino.h>
class ManualModeController
{
private:
  ManualModeController();
  // Thêm biến trạng thái nếu cần
public:
  static ManualModeController &getInstance();

  void setup();
  void run();
  bool cancelSignal();
};
