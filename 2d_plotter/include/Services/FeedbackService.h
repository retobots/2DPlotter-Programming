#pragma once
#include <Arduino.h>
class FeedbackService {
public:
  static FeedbackService& getInstance();

  void setup();
  void showStatus(const char* message);
  void beep(uint8_t type);  // 1: Start, 2: End, 3: Error, etc.

private:
  FeedbackService();
};
