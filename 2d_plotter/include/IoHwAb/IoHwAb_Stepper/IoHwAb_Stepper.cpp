#include "IoHwAb_Stepper.h"

IoHwAb_Stepper::IoHwAb_Stepper()
{
  stepperX = new AccelStepper(AccelStepper::DRIVER, PIN_X_STEP, PIN_X_DIR);
  stepperY = new AccelStepper(AccelStepper::DRIVER, PIN_Y_STEP, PIN_Y_DIR);
}

IoHwAb_Stepper &IoHwAb_Stepper::getInstance()
{
  static IoHwAb_Stepper instance;
  return instance;
}

void IoHwAb_Stepper::setup()
{
  Serial.println("[SET UP]: Stepper Motor Starting Setup!");

  if (stepperX == nullptr || stepperY == nullptr)
  {
    return;
  }

  stepperX->setMaxSpeed(MAX_SPEED);
  stepperX->setAcceleration(ACCELERATION);

  stepperY->setMaxSpeed(MAX_SPEED);
  stepperY->setAcceleration(ACCELERATION);

  // Đảo chiều chân x
  stepperX->setPinsInverted(true, false, false); // Invert direction pin

  // Logging
  Serial.println("[SET UP]: Stepper Motor Done!");
}

void IoHwAb_Stepper::moveTo(float x, float y)
{

  // Tính toán bước và điều khiển motor
  stepperX->moveTo(x);
  stepperY->moveTo(y);

  while (stepperX->distanceToGo() != 0 || stepperY->distanceToGo() != 0)
  {
    stepperX->run();
    stepperY->run();
  }
}

void IoHwAb_Stepper::stop()
{
  // Ngắt tín hiệu step

  stepperX->stop(); // Dừng động cơ X
  stepperY->stop(); // Dừng động cơ Y

  stepperX->disableOutputs(); // Tắt xung (ngắt nguồn giữ motor)
  stepperY->disableOutputs(); // (giúp tiết kiệm điện, tránh nóng motor)
}

// ================= Continuous (non-positional) control =================

void IoHwAb_Stepper::enableOutputs()
{
  stepperX->enableOutputs();
  stepperY->enableOutputs();
}

void IoHwAb_Stepper::setRollSteps(float xStepsPerSec, float yStepsPerSec)
{
  // Constrain to manual roll max (separate from positional MAX_SPEED)
  xStepsPerSec = constrain(xStepsPerSec, -ROLL_MAX_SPEED, ROLL_MAX_SPEED);
  yStepsPerSec = constrain(yStepsPerSec, -ROLL_MAX_SPEED, ROLL_MAX_SPEED);

  stepperX->setSpeed(xStepsPerSec);
  stepperY->setSpeed(yStepsPerSec);
}

void IoHwAb_Stepper::setRollMM(float xMmPerSec, float yMmPerSec)
{
  float xStepsPerSec = xMmPerSec * STEPS_PER_MM_X;
  float yStepsPerSec = yMmPerSec * STEPS_PER_MM_Y;
  setRollSteps(xStepsPerSec, yStepsPerSec);
  // Throttle logging; frequent Serial prints can starve step generation
  static unsigned long lastLog = 0;
  static float lx = 0, ly = 0;
  unsigned long now = millis();
  if (now - lastLog > 250 && (fabs(lx - xStepsPerSec) > 50 || fabs(ly - yStepsPerSec) > 50))
  {
    Serial.print("[ROLL]: xStepsPerSec: ");
    Serial.println(xStepsPerSec);
    Serial.print("[ROLL]: yStepsPerSec: ");
    Serial.println(yStepsPerSec);
    lastLog = now;
    lx = xStepsPerSec;
    ly = yStepsPerSec;
  }
}

void IoHwAb_Stepper::runSpeedTickX()
{

    stepperX->runSpeed();
}


void IoHwAb_Stepper::runSpeedTickY()
{

    stepperY->runSpeed();
}

void IoHwAb_Stepper::stopRolling()
{
  stepperX->setSpeed(0);
  stepperY->setSpeed(0);
}

void IoHwAb_Stepper::setSpeed(float speed)
{
  stepperX->setSpeed(speed);
  stepperY->setSpeed(speed);
}

void IoHwAb_Stepper::setCurrentPosition(long xSteps, long ySteps)
{
  if (stepperX)
    stepperX->setCurrentPosition(xSteps);
  if (stepperY)
    stepperY->setCurrentPosition(ySteps);
}