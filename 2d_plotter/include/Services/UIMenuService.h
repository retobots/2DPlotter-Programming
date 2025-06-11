#pragma once
#include <Arduino.h>
enum class ModeType {
  NONE,
  MANUAL,
  AUTO_SD,
  AUTO_SERIAL
};

class UIMenuService {
public:
  static UIMenuService& getInstance();

  void setup();
  ModeType selectMode(); // Trả về chế độ được chọn

private:
  UIMenuService();
};
