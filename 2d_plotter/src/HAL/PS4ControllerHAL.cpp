#include "PS4ControllerHAL.h"

PS4ControllerHAL::PS4ControllerHAL()
{
  PS4.begin();
}

PS4ControllerHAL &PS4ControllerHAL::getInstance()
{
  static PS4ControllerHAL instance;
  return instance;
}

void PS4ControllerHAL::setup()
{
}

PointData PS4ControllerHAL::readPS4()
{
  PointData points = {0.00, 0.00};

  if (!PS4.isConnected())
    return PointData{};

  int rx = PS4.RStickX();
  int ry = PS4.RStickY();

  // Vùng chết
  if (abs(rx) < SAFE_ZONE_MARGIN && abs(ry) < SAFE_ZONE_MARGIN)
    return PointData{};

  // Tính vector chuẩn hóa
  float magnitude = sqrt(rx * rx + ry * ry);
  float vx = rx / magnitude;
  float vy = ry / magnitude;

  // Di chuyển một đoạn nhỏ theo hướng
  points.x += vx * MAX_MANUAL_SPEED;
  points.y += vy * MAX_MANUAL_SPEED;

  delay(10); // Điều chỉnh mượt tùy tốc độ máy

  // Trả về tọa độ
  return points;
}

bool PS4ControllerHAL::isButtonPressed()
{
  return false;
}
