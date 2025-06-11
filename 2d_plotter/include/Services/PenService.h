#pragma once
#include <Arduino.h>
class PenService {
public:
  static PenService& getInstance();

  void setup();
  void penUp();
  void penDown();
  bool isPenDown();

private:
  PenService();
  bool penDownState;
};
