/*************************************************************************************************************************
 * @file    IoHwAb_Servo.cpp
 * @brief   Implementation of functions to control a servo motor using PWM on ESP32
 * @version 1.0
 * @date    2025-06-19
 * @author  Do Duc Nghia
 ************************************************************************************************************************/

#include "IoHwAb_Servo.h"

/*************************************************************************************************************************
 * @brief Constructor
 ************************************************************************************************************************/
IoHwAb_Servo::IoHwAb_Servo()
{
}

/*************************************************************************************************************************
 * @brief Get the singleton instance
 ************************************************************************************************************************/
IoHwAb_Servo &IoHwAb_Servo::getInstance()
{
  static IoHwAb_Servo instance;
  return instance;
}

/*************************************************************************************************************************
 * @brief Setup the servo motor
 ************************************************************************************************************************/
void IoHwAb_Servo::setup()
{
  Serial.println("[SET UP]: Servo Starting Setup!");

  ledcSetup(servoChannel, 50, 16);
  ledcAttachPin(PIN_SERVO, servoChannel);

  dropPen();

  // Logging
  Serial.println("[SET UP]: Servo Done!");
}

/*************************************************************************************************************************
 * @brief Set the servo angle
 *
 * @param angle Angle in degrees (0-180)
 ************************************************************************************************************************/
void IoHwAb_Servo::setServoAngle(int angle)
{
  angle = constrain(angle, 0, 180);
  int dutyCycle = map(angle, 0, 180, 1638, 8192);
  ledcWrite(servoChannel, dutyCycle);

  Serial.print("[SERVO]: Set angle to ");
  Serial.print(angle);
  Serial.print("° (duty: ");
  Serial.print(dutyCycle);
  Serial.println(")");
}

/*************************************************************************************************************************
 * @brief Lift the pen by setting the servo to the up angle
 ************************************************************************************************************************/
void IoHwAb_Servo::liftPen()
{
  Serial.println("[SERVO]: Lifting pen...");
  setServoAngle(PEN_UP_ANGLE);
  delay(500); // Wait for servo to reach position
}

/*************************************************************************************************************************
 * @brief Drop the pen by setting the servo to the down angle
 ************************************************************************************************************************/
void IoHwAb_Servo::dropPen()
{
  Serial.println("[SERVO]: Dropping pen...");
  setServoAngle(PEN_DOWN_ANGLE);
  delay(500); // Wait for servo to reach position
}
