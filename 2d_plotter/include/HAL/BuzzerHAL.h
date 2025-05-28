#pragma once
#include <Arduino.h>
class BuzzerHAL {
public:
  static BuzzerHAL& getInstance();

  void setup();
  void beepShort();
  void beepLong();
  void beepCustom(uint8_t count, uint16_t duration);

private:
  BuzzerHAL();
};
