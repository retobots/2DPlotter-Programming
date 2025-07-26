#include "IoHwAb_PS4.h"

IoHwAb_PS4::IoHwAb_PS4()
{
  PS4.begin();
}

IoHwAb_PS4 &IoHwAb_PS4::getInstance()
{
  static IoHwAb_PS4 instance;
  return instance;
}

void IoHwAb_PS4::setup()
{
  // Logging
  Serial.println("[SET UP]: PS4 Controller Done!");
}

point IoHwAb_PS4::readPS4()
{
  point points = {0.00, 0.00};

  if (!PS4.isConnected())
    return point{};

  int rx = PS4.RStickX();
  int ry = PS4.RStickY();

  // Vùng chết
  if (abs(rx) < SAFE_ZONE_MARGIN && abs(ry) < SAFE_ZONE_MARGIN)
    return point{};

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

bool IoHwAb_PS4::isButtonPressed()
{
  return false;
}
