#pragma once
#include <Arduino.h>
class AppController {
public:
  static AppController& getInstance();

  void setup();
  void loop();

private:
  AppController();
  // Thêm các biến trạng thái chế độ nếu cần
};
