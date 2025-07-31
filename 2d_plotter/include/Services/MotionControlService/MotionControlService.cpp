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

  Serial.println("[PROCESS]: Moving to position X: " + String(xPos) + ", Y: " + String(yPos));

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

  // Nâng bút trước khi di chuyển
  IoHwAb_Servo::getInstance().liftPen();

  // Gửi lệnh di chuyển
  IoHwAb_Stepper::getInstance().moveTo(xSteps, ySteps);

  // Logging
  Serial.println("[MOVE]: Rapid move to X: " + String(xPos) + ", Y: " + String(yPos));

  // Cập nhật vị trí
  Data.x = xPos;
  Data.y = yPos;
}

void MotionControlService::drawArcCW(float xStart, float yStart, float xEnd, float yEnd, float iOffset, float jOffset)
{
  float cx = xStart + iOffset;
  float cy = yStart + jOffset;

  float radius = sqrt(iOffset * iOffset + jOffset * jOffset);

  float startAngle = atan2(yStart - cy, xStart - cx);
  float endAngle = atan2(yEnd - cy, xEnd - cx);

  // Đảm bảo xoay theo chiều kim đồng hồ
  if (endAngle > startAngle)
    endAngle -= 2 * PI;

  float arcLength = abs(endAngle - startAngle) * radius;
  int segments = max((int)(arcLength / 1.0), 1); // mỗi đoạn ~1mm

  for (int i = 0; i <= segments; ++i)
  {
    float angle = startAngle + (endAngle - startAngle) * (i / (float)segments);
    float x = cx + radius * cos(angle);
    float y = cy + radius * sin(angle);
    drawLine(x, y);
  }

  Data.x = xEnd;
  Data.y = yEnd;
}

void MotionControlService::drawArcCCW(float xStart, float yStart, float xEnd, float yEnd, float iOffset, float jOffset)
{
  float cx = xStart + iOffset;
  float cy = yStart + jOffset;

  float radius = sqrt(iOffset * iOffset + jOffset * jOffset);

  float startAngle = atan2(yStart - cy, xStart - cx);
  float endAngle = atan2(yEnd - cy, xEnd - cx);

  // Đảm bảo xoay ngược chiều kim đồng hồ
  if (endAngle < startAngle)
    endAngle += 2 * PI;

  float arcLength = abs(endAngle - startAngle) * radius;
  int segments = max((int)(arcLength / 1.0), 1); // mỗi đoạn ~1mm

  for (int i = 0; i <= segments; ++i)
  {
    float angle = startAngle + (endAngle - startAngle) * (i / (float)segments);
    float x = cx + radius * cos(angle);
    float y = cy + radius * sin(angle);
    drawLine(x, y);
  }

  Data.x = xEnd;
  Data.y = yEnd;
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