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
  Serial.println("StepperHAL::setup() - start");

  pinMode(ENABLE, OUTPUT);
  Serial.println("pinMode OK");

  digitalWrite(ENABLE, LOW);
  Serial.println("digitalWrite OK");

  if (stepperX == nullptr || stepperY == nullptr)
  {
    Serial.println("Stepper pointer NULL!");
    return;
  }

  stepperX->setMaxSpeed(MAX_SPEED);
  Serial.println("setMaxSpeed X OK");

  stepperX->setAcceleration(ACCELERATION);
  Serial.println("setAccel X OK");

  stepperY->setMaxSpeed(MAX_SPEED);
  Serial.println("setMaxSpeed Y OK");

  stepperY->setAcceleration(ACCELERATION);
  Serial.println("setAccel Y OK");

  Serial.println("StepperHAL::setup() - done");
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
