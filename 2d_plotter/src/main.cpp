#include <Arduino.h>
#include "UIMenuService.h"
#include "GcodeParserService.h"

UIMenuService *UI;
GcodeParserService *GPSer;

point actualPoint;

void setup()
{
  Serial.begin(115200);
  Serial.println("Ready for Setting Up!");

  UI = &UIMenuService::getInstance();
  UI->setup();

  GPSer = &GcodeParserService::getInstance();
  GPSer->setup();

  actualPoint.x = 0;
  actualPoint.y = 0;
  actualPoint.z = 0;
}

void loop()
{
  // UI->run();
  GPSer->readSerial(actualPoint);
}