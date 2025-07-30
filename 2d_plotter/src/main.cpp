#include <Arduino.h>
#include "AppController.h"

AppController &appController = AppController::getInstance();

void setup()
{
    Serial.begin(9600);
    // Khởi tạo các thành phần khác nếu cần
    appController.setup();
}

void loop()
{
    // Chạy ứng dụng chính
    appController.run();
}