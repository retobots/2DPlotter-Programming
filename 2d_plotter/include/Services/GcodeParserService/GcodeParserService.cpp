/************************************************************************************************************************
 * @file     GcodeParserService.cpp
 * @brief    Implementation of G-code parsing service
 * @author   Do Duc Nghia
 * @version  1.1
 ************************************************************************************************************************/
#include "GcodeParserService.h"

/*************************************************************************************************************************
 * @brief   Constructor
 ************************************************************************************************************************/
GcodeParserService::GcodeParserService() {}

/*************************************************************************************************************************
 * @brief   Get the singleton instance of GcodeParserService
 * @return  Reference to the GcodeParserService instance
 ************************************************************************************************************************/
GcodeParserService &GcodeParserService::getInstance()
{
  static GcodeParserService instance;
  return instance;
}

/*************************************************************************************************************************
 * @brief   Setup the G-code Parser Service
 ************************************************************************************************************************/
void GcodeParserService::setup()
{
  // Logging
  Serial.println("[SET UP]: Gcode Parser Done!");
}

/*************************************************************************************************************************
 * @brief   Process an incoming G-code line
 *
 * @param   line      The G-code line to process
 * @param   charNB    Number of characters in the line
 * @param   actualPos Current position of the plotter
 ************************************************************************************************************************/
void GcodeParserService::processIncomingLine(char *line, int charNB, point &actualPos)
{
  Serial.print("Input: ");
  Serial.println(line);

  if (line[0] == '\0' || strcmp(line, "?") == 0)
    return;

  int currentIndex = 0;
  char buffer[32];

  // Parsed values
  int gcode = -1;                     // G00, G01, G02, G03
  point newPos = actualPos;           // New target position
  float zValue = 5.0f;                // Current Z
  float iValue = 0.0f, jValue = 0.0f; // Center offset relative to start (used for G02/G03)
  float feedrate = 0.0f;
  bool hasX = false, hasY = false, hasZ = false, hasI = false, hasJ = false;

  // Pen state is kept across lines
  static bool penDown = false;

  while (currentIndex < charNB && line[currentIndex] != '\0')
  {
    char cmd = line[currentIndex++];

    // Skip spaces and line endings
    if (cmd == ' ' || cmd == '\n' || cmd == '\r')
      continue;

    int i = 0;
    char sign = '+';

    // Optional sign
    if (currentIndex < charNB && (line[currentIndex] == '-' || line[currentIndex] == '+'))
      sign = line[currentIndex++];

    // Read numeric value
    while (currentIndex < charNB &&
           (isdigit(line[currentIndex]) || line[currentIndex] == '.') &&
           i < (int)sizeof(buffer) - 1)
    {
      buffer[i++] = line[currentIndex++];
    }
    buffer[i] = '\0';

    float value = 0.0f;
    if (i > 0)
    {
      value = atof(buffer);
      if (sign == '-')
        value = -value;
    }

    switch (cmd)
    {
    case 'G':
      gcode = (int)value;
      break;

    case 'M':
    {
      int mcode = (int)value;

      switch (mcode)
      {
      case 3:
        IoHwAb_Servo::getInstance().dropPen();
        Serial.println("M3 → Pen Down");
        penDown = true;
        break;

      case 5:
        IoHwAb_Servo::getInstance().liftPen();
        Serial.println("M5 → Pen Up");
        penDown = false;
        break;

      case 2:
        Serial.println("M2 → Program End");
        // Optional: reset state if needed
        break;

      case 114: // M114 – report position
        Serial.print("Position: X=");
        Serial.print(actualPos.x, 3);
        Serial.print(" Y=");
        Serial.print(actualPos.y, 3);
        Serial.print(" Z=");
        Serial.println(0.000, 3);
        break;

      case 300: // M300 S30 / S50 – servo (Inkscape)
        // Parse S parameter
        while (currentIndex < charNB && line[currentIndex] != '\0')
        {
          if (line[currentIndex] == 'S')
          {
            currentIndex++; // Skip 'S'
            int j = 0;
            char sBuffer[10];
            while (currentIndex < charNB &&
                   line[currentIndex] != ' ' &&
                   line[currentIndex] != '\0' &&
                   j < (int)sizeof(sBuffer) - 1)
            {
              sBuffer[j++] = line[currentIndex++];
            }
            sBuffer[j] = '\0';

            if (j > 0)
            {
              int sval = atoi(sBuffer);

              if (sval == 30)
              {
                IoHwAb_Servo::getInstance().dropPen();
                Serial.println("M300 S30 → Pen Down");
                penDown = true;
              }
              else if (sval == 50)
              {
                IoHwAb_Servo::getInstance().liftPen();
                Serial.println("M300 S50 → Pen Up");
                penDown = false;
              }
              else
              {
                Serial.print("M300 unknown S value: ");
                Serial.println(sval);
              }
            }
            break; // Done with S parameter
          }
          else
          {
            currentIndex++; // Skip non-'S' characters
          }
        }
        break;

      default:
        Serial.print("Unsupported M-code: M");
        Serial.println(mcode);
        break;
      }

      break;
    }

    case 'X':
      newPos.x = value;
      hasX = true;
      break;

    case 'Y':
      newPos.y = value;
      hasY = true;
      break;

    case 'Z':
      zValue = value;
      hasZ = true;
      break;

    case 'I':
      iValue = value;
      hasI = true;
      break;

    case 'J':
      jValue = value;
      hasJ = true;
      break;

    case 'F':
      // Feedrate handling:
      //  - We assume F is expressed in mm/min (common for G-code from Inkscape/UGS).
      //  - Convert it to steps/s using X-axis steps/mm (or the dominant axis).
      feedrate = value;
      {
        float feed_mm_per_min = feedrate;
        float feed_mm_per_sec = feed_mm_per_min / 60.0f;
        float steps_per_sec = feed_mm_per_sec * STEPS_PER_MM_X;
        IoHwAb_Stepper::getInstance().setSpeed(steps_per_sec);
      }
      break;

    default:
      // Ignore unsupported/unknown letters
      break;
    }
  }

  // ----------------- Debug output -----------------
  if (gcode >= 0)
  {
    Serial.print("Parsed G-code: G");
    Serial.println(gcode);
  }

  if (hasX || hasY)
  {
    Serial.print("Target: X=");
    Serial.print(newPos.x, 3);
    Serial.print(" Y=");
    Serial.println(newPos.y, 3);
  }

  if (hasZ)
  {
    Serial.print("Z=");
    Serial.println(zValue, 3);
  }

  if (hasI || hasJ)
  {
    Serial.print("Arc center offset: I=");
    Serial.print(iValue, 6);
    Serial.print(" J=");
    Serial.println(jValue, 6);
  }

  // ----------------- Pen control via Z -----------------
  // Only for linear moves (or unspecified G, treated as G0/G1)
  if (hasZ && (gcode == 0 || gcode == 1 || gcode == -1))
  {
    if (zValue < 0 && !penDown)
    {
      IoHwAb_Servo::getInstance().dropPen();
      Serial.println("Z-axis: Pen Down");
      penDown = true;
    }
    else if (zValue > 0 && penDown)
    {
      IoHwAb_Servo::getInstance().liftPen();
      Serial.println("Z-axis: Pen Up");
      penDown = false;
    }
  }

  // ----------------- Execute motion -----------------
  switch (gcode)
  {
  case 0: // G00 - Rapid move (no drawing)
    Serial.print("[DEBUG G0] actualPos.x=");
    Serial.print(actualPos.x, 3);
    Serial.print(" actualPos.y=");
    Serial.print(actualPos.y, 3);
    Serial.print(" targetX=");
    Serial.print(newPos.x, 3);
    Serial.print(" targetY=");
    Serial.println(newPos.y, 3);

    MotionControlService::getInstance().moveTo(newPos.x, newPos.y);
    Serial.print("[G0] Move to X=");
    Serial.print(newPos.x);
    Serial.print(" Y=");
    Serial.println(newPos.y);
    break;

  case 1: // G01 - Draw line
    MotionControlService::getInstance().drawLine(newPos.x, newPos.y);
    Serial.print("Draw line to X=");
    Serial.print(newPos.x);
    Serial.print(" Y=");
    Serial.println(newPos.y);
    break;

  case 2: // G02 - CW arc
    if (hasI && hasJ)
    {
      MotionControlService::getInstance().drawArc(actualPos.x, actualPos.y,
                                                  newPos.x, newPos.y,
                                                  iValue, jValue, true);
      Serial.print("Draw arc CW to X=");
      Serial.print(newPos.x);
      Serial.print(" Y=");
      Serial.print(newPos.y);
      Serial.print(" I=");
      Serial.print(iValue);
      Serial.print(" J=");
      Serial.println(jValue);
    }
    else
    {
      Serial.println("Warning: G02 missing I/J params");
    }
    break;

  case 3: // G03 - CCW arc
    if (hasI && hasJ)
    {
      MotionControlService::getInstance().drawArc(actualPos.x, actualPos.y,
                                                  newPos.x, newPos.y,
                                                  iValue, jValue, false);
      Serial.print("Draw arc CCW to X=");
      Serial.print(newPos.x);
      Serial.print(" Y=");
      Serial.print(newPos.y);
      Serial.print(" I=");
      Serial.print(iValue);
      Serial.print(" J=");
      Serial.println(jValue);
    }
    else
    {
      Serial.println("Warning: G03 missing I/J params");
    }
    break;

  case 21:
    Serial.println("Set units to mm");
    break;

  case 90:
    Serial.println("Set absolute mode (G90)");
    break;

  case 91:
    Serial.println("Set relative mode (G91)");
    break;

  default:
    // gcode == -1 ⇒ line has no explicit G word (e.g. pure M-code or comments)
    // In that case we skip the "unsupported G" message.
    if (gcode != -1)
    {
      Serial.print("Unsupported G-code: G");
      Serial.println(gcode);
    }
    break;
  }

  // Update current position if X/Y were mentioned
  if (hasX || hasY)
  {
    actualPos = newPos;
  }

  Serial.println("ok");
  // IMPORTANT: removed delay(500); to avoid artificial slowdown
}
