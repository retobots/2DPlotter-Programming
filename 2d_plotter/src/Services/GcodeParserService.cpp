#include "Services/GcodeParserService.h"

GcodeParserService::GcodeParserService() {}

GcodeParserService& GcodeParserService::getInstance() {
  static GcodeParserService instance;
  return instance;
}

void GcodeParserService::setup() {
  // Setup nếu cần
}

bool GcodeParserService::parseLine(const std::string& line) {
  // Phân tích lệnh G0/G1/M3/M5
  return true; // placeholder
}
