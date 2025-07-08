#include <Arduino.h>
#include "UIMenuService.h"

UIMenuService *UI;
String inputBuffer = "";

void setup()
{
  delay(3000);
  Serial.begin(9600);
  while (!Serial)
    ; // Đợi kết nối Serial (đặc biệt cho ESP32)
  Serial.println("UART Receiver Ready!");
  UI = &UIMenuService::getInstance();
  UI->setup();
}

void loop()
{
  // UI->run();
  // Kiểm tra có dữ liệu đến không
  while (Serial.available() > 0)
  {
    char c = Serial.read(); // Đọc 1 ký tự

    // Ghi ký tự vào buffer
    if (c != '\n')
    {
      inputBuffer += c;
    }
    else
    {
      // Kết thúc chuỗi
      Serial.print("Received: ");
      Serial.println(inputBuffer);
      inputBuffer = ""; // Xoá buffer để chuẩn bị nhận chuỗi mới
    }
  }
}