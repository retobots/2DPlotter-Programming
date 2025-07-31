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

  // Gắn servo vào chân PWM
  pinMode(PIN_SERVO, OUTPUT);

  // Logging
  Serial.println("[SET UP]: Servo Done!");
}

void IoHwAb_Servo::setServoAngle(int angle)
{
  // Map angle (0-180) to pulse width (500-2500us)
  int pulseWidth = map(angle, 0, 180, 500, 2500);
  // Convert pulse width to duty cycle (for 16-bit, 50Hz)
  for (int i = 0; i < 50; i++)
  { // Send pulses for 1 second
    digitalWrite(PIN_SERVO, HIGH);
    delayMicroseconds(pulseWidth);
    digitalWrite(PIN_SERVO, LOW);
    delayMicroseconds(20000 - pulseWidth);
  }
}

void IoHwAb_Servo::liftPen()
{
  // Góc nâng bút
  setServoAngle(PEN_UP_ANGLE);
  delay(300);
}

void IoHwAb_Servo::dropPen()
{
  // Góc hạ bút
  setServoAngle(PEN_DOWN_ANGLE);
  delay(300);
}
