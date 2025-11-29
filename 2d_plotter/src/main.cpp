#include <Arduino.h>
#include "AppController.h"
#include "IoHwAb_Servo.h"
#include "IoHwAb_Buzzer.h"

void homeMove()
{
  Serial.println("[PROCESS]: Starting Homing Sequence");

  IoHwAb_Stepper &stepper = IoHwAb_Stepper::getInstance();
  IoHwAb_Servo &servo = IoHwAb_Servo::getInstance();

  // Đảm bảo servo bút đã được nâng lên
  servo.liftPen();

  // Tốc độ homing (mm/s) - chỉnh lại cho phù hợp cơ khí của bạn
  const float HOME_SPEED_X_MM_S = -20.0f; // giả sử hướng âm là hướng về công tắc
  const float HOME_SPEED_Y_MM_S = -20.0f;

  // Bật chế độ rolling với tốc độ homing
  stepper.setRollMM(HOME_SPEED_X_MM_S, HOME_SPEED_Y_MM_S);

  bool home_X_done = false;
  bool home_Y_done = false;

  while (!home_X_done || !home_Y_done)
  {
    // HOME Y
    if (!home_Y_done)
    {
      if (digitalRead(PIN_ENDSTOP_Y) == HIGH) // chưa chạm endstop
      {
        stepper.runSpeedTickY();
      }
      else
      {
        home_Y_done = true;
        Serial.println("[HOMING]: Y Axis reached endstop");
      }
    }

    // HOME X
    if (!home_X_done)
    {
      if (digitalRead(PIN_ENDSTOP_X) == HIGH) // chưa chạm endstop
      {
        stepper.runSpeedTickX();
      }
      else
      {
        home_X_done = true;
        Serial.println("[HOMING]: X Axis reached endstop");
      }
    }
  }

  // Dừng rolling
  stepper.stopRolling();

  // Đặt lại vị trí logic về (0,0)
  stepper.setCurrentPosition(0, 0);

  MotionControlService::getInstance().resetData();
  AutoModeController::getInstance().resetData();

  const float DEFAULT_FEED_MM_PER_MIN = 600.0f;
  float feed_mm_per_sec = DEFAULT_FEED_MM_PER_MIN / 60.0f;
  float steps_per_sec = feed_mm_per_sec * STEPS_PER_MM_X;
  IoHwAb_Stepper::getInstance().setSpeed(steps_per_sec);

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
