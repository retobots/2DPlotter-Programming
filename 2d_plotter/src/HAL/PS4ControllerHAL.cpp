#include "PS4ControllerHAL.h"

PS4ControllerHAL::PS4ControllerHAL() {}

PS4ControllerHAL &PS4ControllerHAL::getInstance()
{
  static PS4ControllerHAL instance;
  return instance;
}

void PS4ControllerHAL::setup()
{
  PS4.begin();
}

PointData PS4ControllerHAL::readPS4()
{
  PS4.update();

  PointData points;

  if (!PS4.isConnected())
    return;

  int rx = PS4.RStickX();
  int ry = PS4.RStickY();

  // Vùng chết
  if (abs(rx) < DEADZONE && abs(ry) < DEADZONE)
    return;

  // Tính vector chuẩn hóa
  float magnitude = sqrt(rx * rx + ry * ry);
  float vx = rx / magnitude;
  float vy = ry / magnitude;

  // Di chuyển một đoạn nhỏ theo hướng
  x += vx * SPEED;
  y += vy * SPEED;

  points.x = x;
  points.y = y;

  delay(10); // Điều chỉnh mượt tùy tốc độ máy

  // Trả về tọa độ
  return points;
}

bool PS4ControllerHAL::isButtonPressed()
{
  // Trả về true nếu nút joystick được nhấn
  return false;
}
