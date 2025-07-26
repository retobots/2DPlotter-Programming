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

  // Gắn servo vào chân PWM
  myServo.attach(PIN_SERVO);
  liftPen();

  // Logging
  Serial.println("[SET UP]: Servo Done!");
}

void IoHwAb_Servo::liftPen()
{
  // Góc nâng bút
  myServo.write(PEN_UP_ANGLE);
  delay(300);
}

void IoHwAb_Servo::dropPen()
{
  // Góc hạ bút
  myServo.write(PEN_DOWN_ANGLE);
  delay(300);
}
