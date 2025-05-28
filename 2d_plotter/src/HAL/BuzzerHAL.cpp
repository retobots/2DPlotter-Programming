#include "HAL/BuzzerHAL.h"

BuzzerHAL::BuzzerHAL() {}

BuzzerHAL& BuzzerHAL::getInstance() {
  static BuzzerHAL instance;
  return instance;
}

void BuzzerHAL::setup() {
  // Cấu hình chân GPIO làm output
}

void BuzzerHAL::beepShort() {
  // Bật/tắt beep ngắn
}

void BuzzerHAL::beepLong() {
  // Bật/tắt beep dài
}

void BuzzerHAL::beepCustom(uint8_t count, uint16_t duration) {
  // Bật beep count lần với duration ms
}
