#include "StepperHAL.h"

StepperHAL::StepperHAL()
{
  stepperX = new AccelStepper(AccelStepper::DRIVER, PIN_X_STEP, PIN_X_DIR);
  stepperY = new AccelStepper(AccelStepper::DRIVER, PIN_Y_STEP, PIN_Y_DIR);
}

StepperHAL &StepperHAL::getInstance()
{
  static StepperHAL instance;
  return instance;
}

void StepperHAL::setup()
{
  pinMode(ENABLE, OUTPUT);

  digitalWrite(ENABLE, LOW);

  if (stepperX == nullptr || stepperY == nullptr)
  {
    return;
  }

  stepperX->setMaxSpeed(MAX_SPEED);

  stepperX->setAcceleration(ACCELERATION);

  stepperY->setMaxSpeed(MAX_SPEED);

  stepperY->setAcceleration(ACCELERATION);

  // Logging
  Serial.println("[SET UP]: Stepper Motor Done!");
}

void StepperHAL::moveTo(float x, float y)
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

void StepperHAL::stop()
{
  // Ngắt tín hiệu step

  stepperX->stop(); // Dừng động cơ X
  stepperY->stop(); // Dừng động cơ Y

  stepperX->disableOutputs(); // Tắt xung (ngắt nguồn giữ motor)
  stepperY->disableOutputs(); // (giúp tiết kiệm điện, tránh nóng motor)
}
