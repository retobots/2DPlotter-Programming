#include "HAL/LcdHAL.h"

LcdHAL::LcdHAL() {}

LcdHAL& LcdHAL::getInstance() {
  static LcdHAL instance;
  return instance;
}

void LcdHAL::setup() {
  // Khởi tạo LCD (I2C hoặc SPI)
}

void LcdHAL::print(const std::string& message) {
  // Gửi chuỗi lên LCD
}

void LcdHAL::clear() {
  // Xoá màn hình
}
