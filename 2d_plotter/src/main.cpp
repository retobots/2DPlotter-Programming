#include <Arduino.h>
#include "UIMenuService.h"
#include "AutoModeController.h"

UIMenuService *UI;
AutoModeController *AC;

point actualPoint;

void setup()
{
  Serial.begin(115200);
  Serial.println("Ready for Setting Up!");

  UI = &UIMenuService::getInstance();
  UI->setup();

  AC = &AutoModeController::getInstance();

  actualPoint.x = 0;
  actualPoint.y = 0;
}

void loop()
{
  AC->readSerial(actualPoint);
}