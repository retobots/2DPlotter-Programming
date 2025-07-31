#include "GcodeParserService.h"

GcodeParserService::GcodeParserService() {}

GcodeParserService &GcodeParserService::getInstance()
{
  static GcodeParserService instance;
  return instance;
}

void GcodeParserService::setup()
{
  // Logging
  Serial.println("[SET UP]: Gcode Parser Done!");
}

void GcodeParserService::processIncomingLine(char *line, int charNB, point &actualPos)
{
  Serial.println("[PROCESS]: Processing Incoming Line");

  if (line[0] == '\0' || strcmp(line, "?") == 0)
    return;

  int currentIndex = 0;
  char buffer[32];

  // Trạng thái di chuyển
  int gcode = -1;                   // G00, G01, G02, G03
  point newPos = actualPos;         // Tọa độ đích
  float zValue = 5.0;               // Z hiện tại
  float iValue = 0.0, jValue = 0.0; // Tâm cung tương đối (dùng cho G02/G03)
  float feedrate = 0.0;
  bool hasX = false, hasY = false, hasZ = false, hasI = false, hasJ = false;

  while (currentIndex < charNB && line[currentIndex] != '\0')
  {
    char cmd = line[currentIndex++];

    // Skip spaces
    if (cmd == ' ' || cmd == '\n' || cmd == '\r')
      continue;

    int i = 0;
    char sign = '+';

    if (line[currentIndex] == '-' || line[currentIndex] == '+')
      sign = line[currentIndex++];

    while ((isdigit(line[currentIndex]) || line[currentIndex] == '.') && i < sizeof(buffer) - 1)
      buffer[i++] = line[currentIndex++];
    buffer[i] = '\0';

    float value = atof(buffer);
    if (sign == '-')
      value = -value;

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
      case 3: // M3 – bật bút
        IoHwAb_Servo::getInstance().dropPen();
        Serial.println("M3 → Pen Down");
        break;

      case 5: // M5 – tắt bút
        IoHwAb_Servo::getInstance().liftPen();
        Serial.println("M5 → Pen Up");
        break;

      case 2:
        Serial.println("M2 → Program End");
        // Optional: reset state if needed
        break;

      case 114: // M114 – in vị trí
        Serial.print("Position: X=");
        Serial.print(actualPos.x, 3);
        Serial.print(" Y=");
        Serial.print(actualPos.y, 3);
        Serial.print(" Z=");
        Serial.println(0.000, 3); // Nếu không dùng Z thật
        break;

      case 300: // M300 S30 or S50 – servo control (Inkscape)
        // Đọc ký tự tiếp theo
        while (currentIndex < charNB && line[currentIndex] != '\0')
        {
          if (line[currentIndex++] == 'S')
          {
            int i = 0;
            char sBuffer[10];
            while (currentIndex < charNB &&
                   line[currentIndex] != ' ' &&
                   line[currentIndex] != '\0' &&
                   i < sizeof(sBuffer) - 1)
            {
              sBuffer[i++] = line[currentIndex++];
            }
            sBuffer[i] = '\0';
            int sval = atoi(sBuffer);

            if (sval == 30)
            {
              IoHwAb_Servo::getInstance().dropPen();
              Serial.println("M300 S30 → Pen Down");
            }
            else if (sval == 50)
            {
              IoHwAb_Servo::getInstance().liftPen();
              Serial.println("M300 S50 → Pen Up");
            }
            else
            {
              Serial.print("M300 unknown S value: ");
              Serial.println(sval);
            }
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
      break;
    default:
      // Ignored or unsupported
      break;
    }
  }

  // === Điều khiển bút vẽ ===
  static bool penDown = false;
  if (hasZ)
  {
    if (zValue < 0 && !penDown)
    {
      IoHwAb_Servo::getInstance().dropPen();
      Serial.println("Pen Down");
      penDown = true;
    }
    else if (zValue > 0 && penDown)
    {
      IoHwAb_Servo::getInstance().liftPen();
      Serial.println("Pen Up");
      penDown = false;
    }
  }

  // === Xử lý theo loại G-code ===
  switch (gcode)
  {
  case 0: // G00 - Move nhanh (không vẽ)
    MotionControlService::getInstance().moveTo(newPos.x, newPos.y);
    Serial.print("Move to X=");
    Serial.print(newPos.x);
    Serial.print(" Y=");
    Serial.println(newPos.y);
    break;

  case 1: // G01 - Vẽ đường thẳng
    MotionControlService::getInstance().drawLine(newPos.x, newPos.y);
    Serial.print("Draw line to X=");
    Serial.print(newPos.x);
    Serial.print(" Y=");
    Serial.println(newPos.y);
    break;

  case 2: // G02 - Vẽ cung tròn CW (chiều kim đồng hồ)
    if (hasI && hasJ)
    {
      MotionControlService::getInstance().drawArcCW(actualPos.x, actualPos.y, newPos.x, newPos.y, iValue, jValue);
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

  case 3: // G03 - Vẽ cung tròn CCW (ngược chiều kim đồng hồ)
    if (hasI && hasJ)
    {
      MotionControlService::getInstance().drawArcCCW(actualPos.x, actualPos.y, newPos.x, newPos.y, iValue, jValue);
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
    Serial.print("Unsupported G-code: G");
    Serial.println(gcode);
    break;
  }

  // Cập nhật vị trí hiện tại
  if (hasX || hasY)
  {
    actualPos = newPos;
  }

  Serial.println("ok");
}
