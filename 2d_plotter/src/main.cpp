#include <Arduino.h>
#include "UIMenuService.h"

UIMenuService *UI;

void setup()
{
  Serial.begin(115200);
  Serial.println("Ready for Setting Up!");
  UI = &UIMenuService::getInstance();
  UI->setup();
}

void loop()
{
  UI->run();
}