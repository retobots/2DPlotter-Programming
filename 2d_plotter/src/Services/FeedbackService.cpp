#include "Services/FeedbackService.h"

FeedbackService::FeedbackService() {}

FeedbackService& FeedbackService::getInstance() {
  static FeedbackService instance;
  return instance;
}

void FeedbackService::setup() {
  // Khởi tạo LCD và buzzer
}

void FeedbackService::showStatus(const char* message) {
  // Gửi message lên LCD
}

void FeedbackService::beep(uint8_t type) {
  // Phát âm thanh tùy theo loại
}
