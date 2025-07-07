#include <Arduino.h>
#include <MotionControlService.h>
#include "AutoModeController.h"
#include "ManualModeController.h"

// AutoModeController *AC;
point actualPoint;

void setup()
{
  Serial.begin(112500);

  actualPoint.x = 0;
  actualPoint.y = 0;
}

void loop()
{
}