#include <Arduino.h>
#include "AppController.h"
#include "IoHwAb_Servo.h"
#include "IoHwAb_Buzzer.h"

void homeMove()
{
  Serial.println("[PROCESS]: Starting Homing Sequence");

  // Set speed for homing
  IoHwAb_Stepper::getInstance().setSpeed(400); // Set a slower speed for homing

  // x steps, y steps
  float x = 0.0; // Move negative direction
  float y = 0.0; // Move negative direction
  IoHwAb_Stepper::getInstance().setSpeed(-400);
  // Di chuyển trục Y về vị trí công tắc hành trình
  bool home_X_done = false;
  bool home_Y_done = false;

  while (!home_X_done || !home_Y_done)
  {
    if (digitalRead(PIN_ENDSTOP_Y) == HIGH)
    {
      IoHwAb_Stepper::getInstance().runSpeedTickY(); // Di chuyển ngược trục Y
    }
    else
    {
      home_X_done = true;
    }
    if (digitalRead(PIN_ENDSTOP_X) == HIGH)
    {
      IoHwAb_Stepper::getInstance().runSpeedTickX(); // Di chuyển ngược trục X}
    }
    else
    {
      home_Y_done = true;
    }
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
