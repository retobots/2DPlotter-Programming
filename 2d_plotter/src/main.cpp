#include <Arduino.h>
#include "AppController.h"

void setup()
{
    Serial2.begin(112500); // Khởi tạo Serial với tốc độ 112500 bps
    Serial.begin(115200);  // Khởi tạo Serial với tốc độ 115200 bps
    // Khởi tạo app controller
    AppController::getInstance().setup();

    pinMode(PIN_CLK, INPUT_PULLUP);
}

void loop()
{
    Serial.println("[PROCESS]: Running Application Loop");
    // Chạy ứng dụng chính
    AppController::getInstance().run();
}