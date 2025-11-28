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

  uint32_t deltaPulse_X = (xPos - Data.x) * STEPS_PER_MM_X;
  uint32_t deltaPulse_Y = (yPos - Data.y) * STEPS_PER_MM_Y;
  uint32_t error = 0;

  if (deltaPulse_X >= deltaPulse_Y)
  {
    while (deltaPulse_X > 0)
    {
      IoHwAb_Stepper::getInstance().move(1, 0);
      error = error + deltaPulse_Y;
      if (error >= deltaPulse_X)
      {
        IoHwAb_Stepper::getInstance().move(0, 1);
        error = error - deltaPulse_X;
      }
      deltaPulse_X--;
    }
  }
  else
  {
    while (deltaPulse_Y > 0)
    {
      IoHwAb_Stepper::getInstance().move(0, 1);
      error = error + deltaPulse_X;
      if (error >= deltaPulse_Y)
      {
        IoHwAb_Stepper::getInstance().move(1, 0);
        error = error - deltaPulse_Y;
      }
      deltaPulse_Y--;
    }
  }
  // Cập nhật vị trí hiện tại
  Data.x = xPos;
  Data.y = yPos;
}

void MotionControlService::moveTo(float xPos, float yPos)
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

  // Chuyển đổi mm → step
  long xSteps = xPos * STEPS_PER_MM_X;
  long ySteps = yPos * STEPS_PER_MM_Y;

  // // Nâng bút trước khi di chuyển
  // IoHwAb_Servo::getInstance().liftPen();

  // Gửi lệnh di chuyển
  IoHwAb_Stepper::getInstance().moveTo(xSteps, ySteps);

  // Logging
  Serial.println("[MOVE]: Rapid move to X: " + String(xPos) + ", Y: " + String(yPos));

  // Cập nhật vị trí
  Data.x = xPos;
  Data.y = yPos;
}

void MotionControlService::drawArc(float xStart, float yStart, float xEnd, float yEnd, float iOffset, float jOffset, bool is_clockwise)
{
  float cx = xStart + iOffset;
  float cy = yStart + jOffset;

  float radius = sqrt(iOffset * iOffset + jOffset * jOffset);

  float startAngle = atan2(yStart - cy, xStart - cx);
  float endAngle = atan2(yEnd - cy, xEnd - cx);

  // Đảm bảo xoay theo chiều kim đồng hồ
  if (is_clockwise)
  {
    if (endAngle > startAngle)
      endAngle -= 2 * PI;
  }
  else
  {
    if (endAngle < startAngle)
      endAngle += 2 * PI;
  }

  float arcLength = abs(endAngle - startAngle) * radius;
  int segments = max((int)(arcLength / 1.0), 1); // mỗi đoạn ~1mm

  point *points = new point[segments];

  for (int i = 0; i <= segments; i++)
  {
    float angle = startAngle + (endAngle - startAngle) * (i / (float)segments);
    points[i].x = cx + radius * cos(angle);
    points[i].y = cy + radius * sin(angle);
  }

  for (int i = 0; i <= segments; i++)
  {
    drawLine(points[i].x, points[i].y);
    Data.x = points[i].x;
    Data.y = points[i].y;
  }

  Data.x = xEnd;
  Data.y = yEnd;
  delete[] points;
}

void MotionControlService::stop()
{
  // Dừng mọi chuyển động
  IoHwAb_Stepper::getInstance().stop();
}

void MotionControlService::updateData(point &newData)
{
  Data = newData;
}