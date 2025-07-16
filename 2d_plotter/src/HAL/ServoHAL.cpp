#include "ServoHAL.h"

ServoHAL::ServoHAL()
{
}

ServoHAL &ServoHAL::getInstance()
{
  static ServoHAL instance;
  return instance;
}

void ServoHAL::setup()
{

  // Gắn servo vào chân PWM
  myServo.attach(PIN_SERVO);
  liftPen();

  // Logging
  Serial.println("[SET UP]: Servo Done!");
}

void ServoHAL::liftPen()
{
  // Góc nâng bút
  myServo.write(PEN_UP_ANGLE);
  delay(300);
}

void ServoHAL::dropPen()
{
  // Góc hạ bút
  myServo.write(PEN_DOWN_ANGLE);
  delay(300);
}
