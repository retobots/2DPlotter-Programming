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

  // Logging
  Serial.println("[SET UP]: Manual Controller Done!");
}

void ManualModeController::run()
{
  if (!IoHwAb_PS4::getInstance().isConnected())
  {
    IoHwAb_PS4::getInstance().startConnect();
    return;
  }

  data = IoHwAb_PS4::getInstance().readPS4();
  IoHwAb_PS4::getInstance().liftPen();
  IoHwAb_PS4::getInstance().dropPen();
  IoHwAb_PS4::getInstance().cancel();
  MotionControlService::getInstance().drawLine(data.x, data.y);
}
