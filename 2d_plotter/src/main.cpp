#include <Arduino.h>
#include "AppController.h"

void setup() {
  Serial.begin(115200);
  AppController::getInstance().setup();
}

void loop() {
  AppController::getInstance().loop();
}
