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
  // =========================
  // 1) Sanitize + strip comments
  // =========================
  auto stripCommentsAndNormalize = [](char *s)
  {
    // Remove \r \n early
    for (int k = 0; s[k] != '\0'; k++)
    {
      if (s[k] == '\r' || s[k] == '\n')
      {
        s[k] = '\0';
        break;
      }
    }

    bool inParenComment = false;
    int r = 0;
    int w = 0;

    while (s[r] != '\0')
    {
      char c = s[r++];

      // ( ... ) comment style
      if (inParenComment)
      {
        if (c == ')')
          inParenComment = false;
        continue;
      }
      if (c == '(')
      {
        inParenComment = true;
        continue;
      }

      // ';' comment style (most common)
      if (c == ';')
        break;

      // ',' comment style (only if it looks like " , comment" to avoid killing "X10,Y10")
      if (c == ',')
      {
        char prev = (w > 0) ? s[w - 1] : '\0';
        if (prev == ' ' || prev == '\t')
          break;

        // Otherwise treat comma as separator
        c = ' ';
      }

      // Normalize tab to space
      if (c == '\t')
        c = ' ';

      s[w++] = c;
    }

    s[w] = '\0';

    // Trim trailing spaces
    while (w > 0 && (s[w - 1] == ' '))
    {
      s[w - 1] = '\0';
      w--;
    }
  };

  stripCommentsAndNormalize(line);
  charNB = (int)strlen(line);

  Serial.print("Input: ");
  Serial.println(line);

  if (line[0] == '\0' || strcmp(line, "?") == 0)
    return;

  int currentIndex = 0;
  char buffer[32];

  // Parsed values
  int gcode = -1;
  point newPos = actualPos;
  float zValue = 5.0f;
  float iValue = 0.0f, jValue = 0.0f;
  float feedrate = 0.0f;

  bool hasX = false, hasY = false, hasZ = false, hasI = false, hasJ = false;

  // G4 dwell params
  uint32_t dwellMs = 0;
  bool hasP = false, hasS = false;

  // Pen state is kept across lines
  static bool penDown = false;

  while (currentIndex < charNB && line[currentIndex] != '\0')
  {
    char cmd = line[currentIndex++];

    // Skip spaces
    if (cmd == ' ')
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
        break;

      case 114:
        Serial.print("Position: X=");
        Serial.print(actualPos.x, 3);
        Serial.print(" Y=");
        Serial.print(actualPos.y, 3);
        Serial.print(" Z=");
        Serial.println(0.000, 3);
        break;

      case 300: // M300 S30 / S50
        while (currentIndex < charNB && line[currentIndex] != '\0')
        {
          if (line[currentIndex] == 'S')
          {
            currentIndex++;
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
            break;
          }
          else
          {
            currentIndex++;
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
      feedrate = value;
      {
        float feed_mm_per_sec = (feedrate / 60.0f);
        float steps_per_sec = feed_mm_per_sec * STEPS_PER_MM_X;
        IoHwAb_Stepper::getInstance().setSpeed(steps_per_sec);
      }
      break;

    // ======= G4 dwell =======
    // G4 P100  -> dwell 100 ms
    // G4 S0.5  -> dwell 500 ms
    case 'P':
      dwellMs = (uint32_t)fabs(value);
      hasP = true;
      break;

    case 'S':
      dwellMs = (uint32_t)(fabs(value) * 1000.0f + 0.5f);
      hasS = true;
      break;

    default:
      // ignore unknown
      break;
    }
  }

  // ----------------- Pen control via Z -----------------
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
  case 0:
    MotionControlService::getInstance().moveTo(newPos.x, newPos.y);
    Serial.print("[G0] Move to X=");
    Serial.print(newPos.x);
    Serial.print(" Y=");
    Serial.println(newPos.y);
    break;

  case 1:
    MotionControlService::getInstance().drawLine(newPos.x, newPos.y);
    Serial.print("[G1] Draw line to X=");
    Serial.print(newPos.x);
    Serial.print(" Y=");
    Serial.println(newPos.y);
    break;

  case 2:
    if (hasI && hasJ)
    {
      MotionControlService::getInstance().drawArc(actualPos.x, actualPos.y,
                                                  newPos.x, newPos.y,
                                                  iValue, jValue, true);
      Serial.println("[G2] Draw arc CW");
    }
    else
    {
      Serial.println("Warning: G02 missing I/J params");
    }
    break;

  case 3:
    if (hasI && hasJ)
    {
      MotionControlService::getInstance().drawArc(actualPos.x, actualPos.y,
                                                  newPos.x, newPos.y,
                                                  iValue, jValue, false);
      Serial.println("[G3] Draw arc CCW");
    }
    else
    {
      Serial.println("Warning: G03 missing I/J params");
    }
    break;

  case 4: // ======= G4 dwell =======
  {
    // // Prefer S if both present (rare), else P
    // uint32_t ms = dwellMs;
    // if (!(hasP || hasS))
    //   ms = 0;

    // Serial.print("[G4] Dwell ");
    // Serial.print(ms);
    // Serial.println(" ms");

    // if (ms > 0)
    //   delay(ms); // dwell inherently pauses
    break;
  }

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

  Serial.print("[HEAP] Free heap: ");
  Serial.println(ESP.getFreeHeap());

  Serial.println("ok");
}
