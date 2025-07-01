#include "ManualModeController.h"

ManualModeController::ManualModeController()
{
  MC.setup();
  ps4.setup();
}

ManualModeController &ManualModeController::getInstance()
{
  static ManualModeController instance;
  return instance;
}

void ManualModeController::setup()
{
}

void ManualModeController::run()
{
  paraPoint = ps4.readPS4();
  MC.drawLine(paraPoint.x, paraPoint.y);
}
