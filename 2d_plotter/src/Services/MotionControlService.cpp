#include "Services/MotionControlService.h"

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
  stepper.getInstance();
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
  long x1_steps = xPos * StepsPerMillimeterX;
  long y1_steps = yPos * StepsPerMillimeterY;

  // Đặt mục tiêu cho stepper
  stepper.moveTo(x1_steps, y1_steps);

  // Chạy đồng thời cả 2 động cơ đến vị trí mới
  while (stepperX.distanceToGo() != 0 || stepperY.distanceToGo() != 0)
  {
    stepperX.run();
    stepperY.run();
  }

  // Cập nhật vị trí hiện tại
  Xpos = xPos;
  Ypos = yPos;
}

void MotionControlService::stop()
{
  // Dừng mọi chuyển động
  stepper.stop();
}