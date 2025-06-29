#include "HAL/StepperHAL.h"

StepperHAL::StepperHAL() {}

StepperHAL& StepperHAL::getInstance() {
  static StepperHAL instance;
  return instance;
}

void StepperHAL::setup() {
  // Setup GPIO step/dir cho trục X/Y
}

void StepperHAL::moveTo(float x, float y) {
  // Tính toán bước và điều khiển motor
}

void StepperHAL::stop() {
  // Ngắt tín hiệu step
}
