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
 *
 * NOTE:
 *  - Old implementation used a Bresenham loop and called IoHwAb_Stepper::move()
 *    for every single micro step, which was extremely slow.
 *  - New implementation computes the relative delta in steps once and lets the
 *    timer-driven stepper engine handle the interpolation in real-time.
 ************************************************************************************************************************/
void MotionControlService::drawLine(float xPos, float yPos)
{
  Serial.println("[DRAW]: Drawing line to X: " + String(xPos) + ", Y: " + String(yPos));

  // Clamp target position within allowed workspace
  if (xPos < X_MIN)
    xPos = X_MIN;
  if (xPos > X_MAX)
    xPos = X_MAX;
  if (yPos < Y_MIN)
    yPos = Y_MIN;
  if (yPos > Y_MAX)
    yPos = Y_MAX;

  // Compute relative delta in steps from current position to target
  // (same conversion as before, but we no longer run a Bresenham loop here)
  int32_t dxSteps = static_cast<int32_t>((xPos - Data.x) * STEPS_PER_MM_X);
  int32_t dySteps = static_cast<int32_t>((yPos - Data.y) * STEPS_PER_MM_Y);

  // Delegate the whole move to the stepper engine as a single segment
  IoHwAb_Stepper::getInstance().move(dxSteps, dySteps);

  // Update current position in mm
  Data.x = xPos;
  Data.y = yPos;
}

/*************************************************************************************************************************
 * @brief   Move rapidly to the specified position
 *
 * @param   xPos    Target X position in mm
 * @param   yPos    Target Y position in mm
 *
 * NOTE:
 *  - For now, rapid move (G0) uses the same engine as drawLine(), but it is
 *    still a single timer-driven segment, not a step-by-step loop.
 *  - Feed rate used for this move is controlled via IoHwAb_Stepper::setSpeed()
 *    by the G-code parser (F word handling).
 ************************************************************************************************************************/
void MotionControlService::moveTo(float xPos, float yPos)
{
  // Clamp target position within allowed workspace
  if (xPos < X_MIN)
    xPos = X_MIN;
  if (xPos > X_MAX)
    xPos = X_MAX;
  if (yPos < Y_MIN)
    yPos = Y_MIN;
  if (yPos > Y_MAX)
    yPos = Y_MAX;

  // Compute relative delta in steps from current position to target
  int32_t dxSteps = static_cast<int32_t>((xPos - Data.x) * STEPS_PER_MM_X);
  int32_t dySteps = static_cast<int32_t>((yPos - Data.y) * STEPS_PER_MM_Y);

  // Execute rapid move as a single segment
  IoHwAb_Stepper::getInstance().move(dxSteps, dySteps);

  // Logging
  Serial.println("[MOVE]: Rapid move to X: " + String(xPos) + ", Y: " + String(yPos));

  // Update current position in mm
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

  // Ensure correct rotation direction
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
  int segments = max((int)(arcLength / 1.0), 1); // approx. 1 mm per segment

  point *points = new point[segments];

  for (int i = 0; i < segments; i++)
  {
    float t = static_cast<float>(i) / static_cast<float>(segments);
    float angle = startAngle + (endAngle - startAngle) * t;
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
