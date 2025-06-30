#pragma once
#include <Arduino.h>

typedef struct point
{
  float x;
  float y;
  float z;
} point;

// extern point actualPoint;

class MotionControlService
{
public:
  static MotionControlService &getInstance();

  void setup();
  void moveTo(float x, float y);
  void stop();

private:
  MotionControlService();
};
