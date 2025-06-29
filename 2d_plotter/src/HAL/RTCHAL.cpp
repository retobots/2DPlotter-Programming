#include "HAL/RTCHAL.h"

RTCHAL::RTCHAL() {}

RTCHAL& RTCHAL::getInstance() {
  static RTCHAL instance;
  return instance;
}

void RTCHAL::setup() {
  // Khởi động DS1307
}

std::string RTCHAL::getCurrentTime() {
  // Trả về chuỗi thời gian hiện tại
  return "00:00";
}
