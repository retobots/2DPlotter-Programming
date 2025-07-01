#include "GcodeParserService.h"

GcodeParserService::GcodeParserService() {}

GcodeParserService &GcodeParserService::getInstance()
{
  static GcodeParserService instance;
  return instance;
}

void GcodeParserService::setup()
{
  pinMode(2, OUTPUT);

  digitalWrite(2, HIGH); // Bật LED
  delay(500);
  digitalWrite(2, LOW); // Tắt LED
  delay(500);
}

void GcodeParserService::processIncomingLine(char *line, int charNB, point &actualPos)
{
  // Kiểm tra lệnh rỗng hoặc lệnh trạng thái GRBL
  if (line[0] == '\0' || strcmp(line, "?") == 0)
  {
    return; // Bỏ qua lệnh trống hoặc lệnh ? (đã xử lý trong readSerial)
  }

  int currentIndex = 0;
  char buffer[128];               // Tăng kích thước buffer để an toàn
  point newPos = {0.0, 0.0, 0.0}; // Khởi tạo vị trí mới

  while (currentIndex < charNB && line[currentIndex] != '\0')
  {
    switch (line[currentIndex++])
    {
    case 'U':
      // penUp();
      Serial.println("Pen Up");
      break;
    case 'D':
      // penDown();
      Serial.println("Pen Down");
      break;
    case 'G':
    {
      // Đọc số lệnh G (hỗ trợ 1 hoặc 2 chữ số)
      int cmdIndex = 0;
      while (currentIndex < charNB && isdigit(line[currentIndex]) && cmdIndex < 2)
      {
        buffer[cmdIndex++] = line[currentIndex++];
      }
      buffer[cmdIndex] = '\0';

      switch (atoi(buffer))
      {
      case 0: // G00
      case 1: // G01
      {
        newPos.x = actualPos.x;
        newPos.y = actualPos.y;

        // Phân tích tham số X, Y
        while (currentIndex < charNB && line[currentIndex] != '\0')
        {
          char param = line[currentIndex++];
          if (param == 'X' || param == 'Y')
          {
            cmdIndex = 0;
            while (currentIndex < charNB && line[currentIndex] != ' ' && line[currentIndex] != '\0')
            {
              buffer[cmdIndex++] = line[currentIndex++];
            }
            buffer[cmdIndex] = '\0';
            float value = atof(buffer);
            if (param == 'X')
              newPos.x = value;
            else if (param == 'Y')
              newPos.y = value;
          }
          else
          {
            // Đặt đoạn này ở đây
            if (param == 'F' || param == 'S' || param == 'Z')
            {
              while (currentIndex < charNB && line[currentIndex] != ' ' && line[currentIndex] != '\0')
                currentIndex++;
            }
            else
            {
              while (currentIndex < charNB && line[currentIndex] != ' ' && line[currentIndex] != '\0')
                currentIndex++;
            }
          }
        }

        // drawLine(newPos.x, newPos.y);
        actualPos.x = newPos.x;
        actualPos.y = newPos.y;
        Serial.println("Draw Line");
        break;
      }
      default:
        Serial.print("G command not supported: G");
        Serial.println(buffer);
        break;
      }
    }
    break;
    case 'M':
    {
      // Đọc số lệnh M (hỗ trợ 1-3 chữ số)
      int cmdIndex = 0;
      while (currentIndex < charNB && isdigit(line[currentIndex]) && cmdIndex < 3)
      {
        buffer[cmdIndex++] = line[currentIndex++];
      }
      buffer[cmdIndex] = '\0';

      switch (atoi(buffer))
      {
      case 300:
      {
        float Spos = 0.0;
        while (currentIndex < charNB && line[currentIndex] != '\0')
        {
          if (line[currentIndex++] == 'S')
          {
            cmdIndex = 0;
            while (currentIndex < charNB && line[currentIndex] != ' ' && line[currentIndex] != '\0')
            {
              buffer[cmdIndex++] = line[currentIndex++];
            }
            buffer[cmdIndex] = '\0';
            Spos = atof(buffer);
          }
        }
        if (Spos == 30)
        {
          // penDown();
          Serial.println("Pen Down");
        }
        else if (Spos == 50)
        {
          // penUp();
          Serial.println("Pen Up");
        }
        else
        {
          Serial.print("Invalid S value for M300: ");
          Serial.println(Spos);
        }
        break;
      }
      case 114: // M114 - Report position
        Serial.print("Absolute position : X = ");
        Serial.print(actualPos.x);
        Serial.print("  -  Y = ");
        Serial.println(actualPos.y);
        break;
      default:
        Serial.print("Command not recognized : M");
        Serial.println(buffer);
        break;
      }
    }
    break;
    default:
      Serial.print("Command not recognized: ");
      Serial.println(line[currentIndex - 1]);
      break;
    }
  }

  // Chỉ gửi một "ok" duy nhất sau khi xử lý xong lệnh
  Serial.println("ok");
}
