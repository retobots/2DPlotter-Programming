#include <Arduino.h>
#include "AppController.h"
#include "IoHwAb_Servo.h"
#include "IoHwAb_Buzzer.h"

void setup()
{
    Serial2.begin(112500); // Khởi tạo Serial với tốc độ 112500 bps
    Serial.begin(9600);  // Khởi tạo Serial với tốc độ 115200 bps

    // Khởi tạo app controller
    AppController::getInstance().setup();
}

void loop()
{
    Serial.println("[PROCESS]: Running Application Loop");
    // Chạy ứng dụng chính
    AppController::getInstance().run();
}
