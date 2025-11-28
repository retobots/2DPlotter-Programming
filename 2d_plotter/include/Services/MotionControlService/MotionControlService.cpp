/************************************************************************************************************************
 * @file     MotionControlService.cpp
 * @brief    Definitions of functions in the source file
 * @author   Do Duc Nghia
 * @version  1.0
 ************************************************************************************************************************/

#include "MotionControlService.h"

/*************************************************************************************************************************
 * @brief   Constructor
 ************************************************************************************************************************/
MotionControlService::MotionControlService()
{
}

/*************************************************************************************************************************
 * @brief   Get the singleton instance of MotionControlService
 * @return  Reference to the MotionControlService instance
 ************************************************************************************************************************/
MotionControlService &MotionControlService::getInstance()
{
  static MotionControlService instance;
  return instance;
}

/*************************************************************************************************************************
 * @brief   Setup the Motion Control Service
 ************************************************************************************************************************/
void MotionControlService::setup()
{
  IoHwAb_Stepper::getInstance().setup();

  // Logging
  Serial.println("[SET UP]: Motion Control Done!");
}

/*************************************************************************************************************************
 * @brief   Draw a line to the specified position
 *
 * @param   xPos    Target X position in mm
 * @param   yPos    Target Y position in mm
 ************************************************************************************************************************/
void MotionControlService::drawLine(float xPos, float yPos)
{
  Serial.println("[DRAW]: Drawing line to X: " + String(xPos) + ", Y: " + String(yPos));
  // Giới hạn trong vùng cho phép
  if (xPos < X_MIN)
    xPos = X_MIN;
  if (xPos > X_MAX)
    xPos = X_MAX;
  if (yPos < Y_MIN)
    yPos = Y_MIN;
  if (yPos > Y_MAX)
    yPos = Y_MAX;

  // Calculate the difference in steps
  int32_t dx = (xPos - Data.x) * STEPS_PER_MM_X;
  int32_t dy = (yPos - Data.y) * STEPS_PER_MM_Y;

  int stepDirX = (dx >= 0) ? 1 : -1;
  int stepDirY = (dy >= 0) ? 1 : -1;

  uint32_t deltaPulse_X = abs(dx);
  uint32_t deltaPulse_Y = abs(dy);
  uint32_t error = 0;
  uint32_t stepCount = 0;

  if (deltaPulse_X >= deltaPulse_Y)
  {
    while (deltaPulse_X > 0)
    {
      IoHwAb_Stepper::getInstance().move(stepDirX, 0);
      error += deltaPulse_Y;
      if (error >= deltaPulse_X)
      {
        IoHwAb_Stepper::getInstance().move(0, stepDirY);
        error -= deltaPulse_X;
      }
      deltaPulse_X--;
      stepCount++;
    }
  }
  else
  {
    while (deltaPulse_Y > 0)
    {
      IoHwAb_Stepper::getInstance().move(0, stepDirY);
      error += deltaPulse_X;
      if (error >= deltaPulse_Y)
      {
        IoHwAb_Stepper::getInstance().move(stepDirX, 0);
        error -= deltaPulse_Y;
      }
      deltaPulse_Y--;
      stepCount++;
    }
  }
  // Update current position
  Data.x = xPos;
  Data.y = yPos;
}

/*************************************************************************************************************************
 * @brief   Move rapidly to the specified position
 *
 * @param   xPos    Target X position in mm
 * @param   yPos    Target Y position in mm
 ************************************************************************************************************************/
void MotionControlService::moveTo(float xPos, float yPos)
{
  // Deadzone check
  if (xPos < X_MIN)
    xPos = X_MIN;
  if (xPos > X_MAX)
    xPos = X_MAX;
  if (yPos < Y_MIN)
    yPos = Y_MIN;
  if (yPos > Y_MAX)
    yPos = Y_MAX;

  // Convert mm to steps
  long xSteps = xPos * STEPS_PER_MM_X;
  long ySteps = yPos * STEPS_PER_MM_Y;

  // Send move command
  IoHwAb_Stepper::getInstance().moveTo(xSteps, ySteps);

  // Logging
  Serial.println("[MOVE]: Rapid move to X: " + String(xPos) + ", Y: " + String(yPos));

  // Update current position
  Data.x = xPos;
  Data.y = yPos;
}

/*************************************************************************************************************************
 * @brief   Draw an arc from start to end position with given center offsets
 *
 * @param   xStart        Starting X position in mm
 * @param   yStart        Starting Y position in mm
 * @param   xEnd          Ending X position in mm
 * @param   yEnd          Ending Y position in mm
 * @param   iOffset       Center offset in X from start position in mm
 * @param   jOffset       Center offset in Y from start position in mm
 * @param   is_clockwise  Direction of arc (true for clockwise, false for counter-clockwise)
 ************************************************************************************************************************/
void MotionControlService::drawArc(float xStart, float yStart, float xEnd, float yEnd, float iOffset, float jOffset, bool is_clockwise)
{
  float cx = xStart + iOffset;
  float cy = yStart + jOffset;

  float radius = sqrt(iOffset * iOffset + jOffset * jOffset);

  float startAngle = atan2(yStart - cy, xStart - cx);
  float endAngle = atan2(yEnd - cy, xEnd - cx);

  // Ensure clockwise rotation
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

  for (int i = 0; i < segments; i++)
  {
    float angle = startAngle + (endAngle - startAngle) * (i / (float)segments);
    points[i].x = cx + radius * cos(angle);
    points[i].y = cy + radius * sin(angle);
  }

  for (int i = 0; i < segments; i++)
  {
    drawLine(points[i].x, points[i].y);
    Data.x = points[i].x;
    Data.y = points[i].y;
  }

  Data.x = xEnd;
  Data.y = yEnd;
  delete[] points;
}

/*************************************************************************************************************************
 * @brief   Stop all motion
 ************************************************************************************************************************/
void MotionControlService::stop()
{
  // Stop all motion
  IoHwAb_Stepper::getInstance().stop();
}

/*************************************************************************************************************************
 * @brief   Update internal data with new position
 *
 * @param   newData   New position data
 ************************************************************************************************************************/
void MotionControlService::updateData(point &newData)
{
  Data = newData;
}