#include "HAL/RotaryEncoderHAL.h"

RotaryEncoderHAL::RotaryEncoderHAL() {}

RotaryEncoderHAL& RotaryEncoderHAL::getInstance() {
  static RotaryEncoderHAL instance;
  return instance;
}

void RotaryEncoderHAL::setup() {
  // Cấu hình encoder
}

int RotaryEncoderHAL::readRotation() {
  // Trả về hướng xoay
  return 0;
}

bool RotaryEncoderHAL::isButtonPressed() {
  // Trả về trạng thái nút nhấn trên encoder
  return false;
}
