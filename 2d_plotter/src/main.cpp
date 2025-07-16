#include <Arduino.h>
#include "UIMenuService.h"
#include "AutoModeController.h"

// Khởi tạo đối tượng quản lý
AutoModeController *AC;
UIMenuService *UI;

void setup()
{
  delay(3000);
  Serial.begin(9600);
  while (!Serial)
    ; // Đợi kết nối Serial (đặc biệt cho ESP32)
  Serial.println("UART Receiver Ready!");

  // Setup
  Serial.println("================ [ SETTING - UP ] ===============");
  AC->setup();
  UI->setup();
}

void loop()
{
  AC->run();
}