#include <Arduino.h>
#include "AppController.h"
#include "IoHwAb_Servo.h"
#include "IoHwAb_Buzzer.h"

void homeMove()
{
  Serial.println("[PROCESS]: Starting Homing Sequence");

  // Set speed for homing
  IoHwAb_Stepper::getInstance().setSpeed(-400); // Set a slower speed for homing

  // Di chuyển trục Y về vị trí công tắc hành trình
  while (digitalRead(PIN_ENDSTOP_Y) == HIGH)
  {
    IoHwAb_Stepper::getInstance().runSpeedTick(); // Di chuyển ngược trục Y
    Serial.println("[PROCESS]: Homing Y Axis...");
  }
  IoHwAb_Stepper::getInstance().setCurrentPosition(0, 0);
  IoHwAb_Stepper::getInstance().moveTo(0, 0);
  Serial.println("[PROCESS]: Y Axis Homed");

  // Di chuyển trục X về vị trí công tắc hành trình
  while (digitalRead(PIN_ENDSTOP_X) == HIGH)
  {
    IoHwAb_Stepper::getInstance().runSpeedTick(); // Di chuyển ngược trục X
    Serial.println("[PROCESS]: Homing X Axis...");
  }
  IoHwAb_Stepper::getInstance().setCurrentPosition(0, 0);
  IoHwAb_Stepper::getInstance().moveTo(0, 0);
  Serial.println("[PROCESS]: X Axis Homed");

  Serial.println("[PROCESS]: Homing Sequence Completed");
}

void setup()
{
  // Serial2.begin(112500); // Khởi tạo Serial với tốc độ 112500 bps
  Serial.begin(9600); // Khởi tạo Serial với tốc độ 115200 bps

  pinMode(PIN_ENDSTOP_X, INPUT);
  pinMode(PIN_ENDSTOP_Y, INPUT);
  // Khởi tạo app controller
  AppController::getInstance().setup();

  // Thực hiện di chuyển về home
  homeMove();
}

void loop()
{
  Serial.println("[PROCESS]: Running Application Loop");
  // // Chạy ứng dụng chính
  AppController::getInstance().run();
}
