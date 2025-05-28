#include "HAL/ServoHAL.h"

ServoHAL::ServoHAL() {}

ServoHAL& ServoHAL::getInstance() {
  static ServoHAL instance;
  return instance;
}

void ServoHAL::setup() {
  // Gắn servo vào chân PWM
}

void ServoHAL::liftPen() {
  // Góc nâng bút
}

void ServoHAL::dropPen() {
  // Góc hạ bút
}
