#include "IoHwAb_Servo.h"

IoHwAb_Servo::IoHwAb_Servo()
{
}

IoHwAb_Servo &IoHwAb_Servo::getInstance()
{
  static IoHwAb_Servo instance;
  return instance;
}

void IoHwAb_Servo::setup()
{
  Serial.println("[SET UP]: Servo Starting Setup!");

  // Configure PWM for servo control
  // frequency: 50Hz, resolution: 16-bit, channel: 0
  ledcSetup(servoChannel, 50, 16); // 50Hz, 16-bit resolution
  ledcAttachPin(PIN_SERVO, servoChannel);

  // Initialize servo to middle position
  dropPen();

  // Logging
  Serial.println("[SET UP]: Servo Done!");
}

void IoHwAb_Servo::setServoAngle(int angle)
{
  // Clamp angle to valid range
  angle = constrain(angle, 0, 180);

  // Map angle (0-180) to duty cycle (1638-8192 for 16-bit, 50Hz)
  // 1ms = 1638, 1.5ms = 4915, 2ms = 8192
  int dutyCycle = map(angle, 0, 180, 1638, 8192);

  // Set PWM duty cycle - this maintains the signal continuously
  ledcWrite(servoChannel, dutyCycle);

  Serial.print("[SERVO]: Set angle to ");
  Serial.print(angle);
  Serial.print("° (duty: ");
  Serial.print(dutyCycle);
  Serial.println(")");
}

void IoHwAb_Servo::liftPen()
{
  Serial.println("[SERVO]: Lifting pen...");
  setServoAngle(PEN_UP_ANGLE);
  delay(500); // Wait for servo to reach position
}

void IoHwAb_Servo::dropPen()
{
  Serial.println("[SERVO]: Dropping pen...");
  setServoAngle(PEN_DOWN_ANGLE);
  delay(500); // Wait for servo to reach position
}
