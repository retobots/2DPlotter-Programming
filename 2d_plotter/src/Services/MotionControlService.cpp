#include "MotionControlService.h"

MotionControlService::MotionControlService()
{
  setup();
}

MotionControlService &MotionControlService::getInstance()
{
  static MotionControlService instance;
  return instance;
}

void MotionControlService::setup()
{
  // Khởi tạo stepper
  stepper.setup();
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
  stepper.moveTo(x1_steps, y1_steps);

  // Cập nhật vị trí hiện tại
  Xpos = xPos;
  Ypos = yPos;
}

void MotionControlService::stop()
{
  // Dừng mọi chuyển động
  stepper.stop();
}