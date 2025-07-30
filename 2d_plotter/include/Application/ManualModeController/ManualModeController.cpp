#include "ManualModeController.h"

ManualModeController::ManualModeController()
{
}

ManualModeController &ManualModeController::getInstance()
{
  static ManualModeController instance;
  return instance;
}

void ManualModeController::setup()
{
  IoHwAb_PS4::getInstance().setup();
  MotionControlService::getInstance().setup();

  // Logging
  Serial.println("[SET UP]: Manual Controller Done!");
}

void ManualModeController::run()
{
  data = IoHwAb_PS4::getInstance().readPS4();
  MotionControlService::getInstance().drawLine(data.x, data.y);
}
