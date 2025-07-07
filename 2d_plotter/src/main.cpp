#include <Arduino.h>
#include <MotionControlService.h>
#include "AutoModeController.h"
#include "ManualModeController.h"

AutoModeController *AC;
point actualPoint;

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ;
  AC = &AutoModeController::getInstance();
  AC->setup();

  actualPoint.x = 0;
  actualPoint.y = 0;
}

void loop()
{
  AC->readSerial();
  if (Serial.available() > 0)
  {
    Serial.println();
  }
}