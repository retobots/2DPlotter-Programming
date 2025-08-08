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

  // Read stick as velocity hint (mm/s)
  data = IoHwAb_PS4::getInstance().readPS4();

  // Optional deadzone smoothing to avoid tiny jitters
  auto dz = [](float v, float dead)
  { return (fabs(v) < dead) ? 0.0f : v; };
  data.x = dz(data.x, 0.005f);
  data.y = dz(data.y, 0.005f);

  float xMmPerSec = data.x * 100.0f;
  float yMmPerSec = data.y * 100.0f;

  // Debug prints here can drastically reduce stepping throughput; keep disabled in manual drive
  // Serial.print("[MANUAL]: xMmPerSec: "); Serial.println(xMmPerSec);
  // Serial.print("[MANUAL]: yMmPerSec: "); Serial.println(yMmPerSec);

  // Apply
  if (fabs(xMmPerSec) < 0.01f && fabs(yMmPerSec) < 0.01f)
  {
    IoHwAb_Stepper::getInstance().stopRolling();
  }
  else
  {
    IoHwAb_Stepper::getInstance().enableOutputs();
    IoHwAb_Stepper::getInstance().setRollMM(xMmPerSec, yMmPerSec);
  }

  // Tick multiple times to maximize achievable step rate within this loop iteration
  for (int i = 0; i < 5; ++i)
  {
    IoHwAb_Stepper::getInstance().runSpeedTick();
  }

  // Buttons
  IoHwAb_PS4::getInstance().liftPen();
  IoHwAb_PS4::getInstance().dropPen();
  IoHwAb_PS4::getInstance().cancel();
}
