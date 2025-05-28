#include "Services/MotionControlService.h"

MotionControlService::MotionControlService() {}

MotionControlService& MotionControlService::getInstance() {
  static MotionControlService instance;
  return instance;
}

void MotionControlService::setup() {
  // Khởi tạo stepper nếu cần
}

void MotionControlService::moveTo(float x, float y) {
  // Tính toán và điều khiển di chuyển stepper
}

void MotionControlService::stop() {
  // Dừng mọi chuyển động
}
