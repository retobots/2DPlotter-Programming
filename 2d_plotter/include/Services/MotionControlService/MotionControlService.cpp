#include "MotionControlService.h"

MotionControlService::MotionControlService()
{
}

MotionControlService &MotionControlService::getInstance()
{
  static MotionControlService instance;
  return instance;
}

void MotionControlService::setup()
{
  // Khởi tạo stepper
  IoHwAb_Stepper::getInstance().setup();

  // Logging
  Serial.println("[SET UP]: Motion Control Done!");
}

void MotionControlService::drawLine(float xPos, float yPos)
{
  // Giới hạn trong vùng cho phép
  if (xPos < X_MIN)
    xPos = X_MIN;
  if (xPos > X_MAX)
    xPos = X_MAX;
  if (yPos < Y_MIN)
    yPos = Y_MIN;
  if (yPos > Y_MAX)
    yPos = Y_MAX;

  // Chuyển đổi mm -> step
  long x1_steps = xPos * STEPS_PER_MM_X;
  long y1_steps = yPos * STEPS_PER_MM_Y;

  // Chạy tới vị trí mới
  IoHwAb_Stepper::getInstance().moveTo(x1_steps, y1_steps);

  // Cập nhật vị trí hiện tại
  Data.x = xPos;
  Data.y = yPos;
}

void MotionControlService::stop()
{
  // Dừng mọi chuyển động
  IoHwAb_Stepper::getInstance().stop();
}

void MotionControlService::updateData(point &newData)
{
  newData = Data;
}