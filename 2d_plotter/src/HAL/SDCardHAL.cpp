#include "HAL/SDCardHAL.h"

SDCardHAL::SDCardHAL() {}

SDCardHAL& SDCardHAL::getInstance() {
  static SDCardHAL instance;
  return instance;
}

void SDCardHAL::setup() {
  // Mount thẻ SD
}

std::vector<std::string> SDCardHAL::listFiles() {
  // Trả về danh sách file .gcode
  return {};
}

bool SDCardHAL::openFile(const std::string& filename) {
  // Mở file
  return true;
}

std::string SDCardHAL::readLine() {
  // Đọc 1 dòng G-code
  return "";
}

void SDCardHAL::closeFile() {
  // Đóng file
}
