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
}

void ManualModeController::run()
{
  paraPoint = ps4.readPS4();
  MC.drawLine(paraPoint.x, paraPoint.y);
}
