#include "Services/PenService.h"

PenService::PenService() : penDownState(false) {}

PenService& PenService::getInstance() {
  static PenService instance;
  return instance;
}

void PenService::setup() {
  // Khởi tạo servo
}

void PenService::penUp() {
  // Nâng bút
  penDownState = false;
}

void PenService::penDown() {
  // Hạ bút
  penDownState = true;
}

bool PenService::isPenDown() {
  return penDownState;
}
