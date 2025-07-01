#include "HAL/StepperHAL.h"

StepperHAL::StepperHAL() {}

StepperHAL &StepperHAL::getInstance()
{
  static StepperHAL instance;
  return instance;
}

void StepperHAL::setup()
{
  // Setup GPIO step/dir cho trục X/Y
  // Setup Chân Điều Khiển X
  AccelStepper stepperX(AccelStepper::DRIVER, PIN_X_STEP, PIN_X_DIR);

  // Setup chân điều khiển Y
  AccelStepper stepperY(AccelStepper::DRIVER, PIN_Y_STEP, PIN_Y_DIR);

  // Set Thông Số
  pinMode(ENABLE, OUTPUT);
  digitalWrite(ENABLE, LOW);

  stepperX.setMaxSpeed(MAX_SPEED);        // Tốc độ tối đa (step/giây)
  stepperX.setAcceleration(ACCELERATION); // Gia tốc (step/giây^2)

  stepperY.setMaxSpeed(MAX_SPEED);        // Tốc độ tối đa (step/giây)
  stepperY.setAcceleration(ACCELERATION); // Gia tốc (step/giây^2)
}

void StepperHAL::moveTo(float x, float y)
{
  // Tính toán bước và điều khiển motor
  stepperX.moveTo(x);
  stepperY.moveTo(y);

  while (stepperX.distanceToGo() != 0 || stepperY.distanceToGo() != 0)
  {
    stepperX.run();
    stepperY.run();
  }
}

void StepperHAL::stop()
{
  // Ngắt tín hiệu step

  stepperX.stop(); // Dừng động cơ X
  stepperY.stop(); // Dừng động cơ Y

  stepperX.disableOutputs(); // Tắt xung (ngắt nguồn giữ motor)
  stepperY.disableOutputs(); // (giúp tiết kiệm điện, tránh nóng motor)
}
