#include "ManualModeController.h"

ManualModeController::ManualModeController() : ps4(PS4ControllerHAL::getInstance()), MC(MotionControlService::getInstance())
{
}

ManualModeController &ManualModeController::getInstance()
{
  static ManualModeController instance;
  return instance;
}

void ManualModeController::setup()
{
  MC.setup();
  ps4.setup();

  // Logging
  Serial.println("[SET UP]: Manual Controller Done!");
}

void ManualModeController::run()
{
  data = ps4.readPS4();
  MC.drawLine(data.x, data.y);
}
