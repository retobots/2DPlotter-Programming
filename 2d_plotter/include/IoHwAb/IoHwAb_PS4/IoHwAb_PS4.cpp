#include "IoHwAb_PS4.h"

IoHwAb_PS4::IoHwAb_PS4()
{
}

IoHwAb_PS4 &IoHwAb_PS4::getInstance()
{
  static IoHwAb_PS4 instance;
  return instance;
}

void IoHwAb_PS4::setup()
{
  Serial.println("[SET UP]: PS4 Controller Starting Setup!");

  PS4.begin();
  // Logging
  Serial.println("[SET UP]: PS4 Controller Done!");
}

void IoHwAb_PS4::startConnect()
{
  if (!PS4.isConnected())
  {
    Serial.println("[PS4]: Starting connection...");
    Serial.println("MAC Address: ");
    printDeviceAddress();
    UIMenuService::getInstance().PS4ModeScreen(0);
  }
}

void IoHwAb_PS4::liftPen()
{
  if (PS4.R1() && PS4.isConnected())
  {
    IoHwAb_Servo::getInstance().liftPen();
    Serial.println("[PS4]: Pen lifted");
  }
}

void IoHwAb_PS4::dropPen()
{
  if (PS4.L1() && PS4.isConnected())
  {
    IoHwAb_Servo::getInstance().dropPen();
    Serial.println("[PS4]: Pen dropped");
  }
}

void IoHwAb_PS4::cancel()
{
  if (PS4.Touchpad() && PS4.isConnected())
  {
    IoHwAb_Servo::getInstance().liftPen();
    Serial.println("[PS4]: Pen lifted (cancel)");
    removePairedDevices();
    PS4.end();
    Serial.println("[PS4]: Disconnected");
    points = {0.00, 0.00}; // Reset points
    IoHwAb_LCD::getInstance().clear();
    UIMenuService::getInstance().PS4ModeScreen(2);
  }
}

point IoHwAb_PS4::readPS4()
{
  if (!PS4.isConnected())
    return point{};

  int rx = PS4.RStickX();
  int ry = PS4.RStickY();

  Serial.print("[PS4]: RStickX: ");
  Serial.print(rx);
  Serial.print(" RStickY: ");
  Serial.println(ry);

  // Vùng chết
  if (abs(rx) < SAFE_ZONE_MARGIN && abs(ry) < SAFE_ZONE_MARGIN)
    return points;

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

void IoHwAb_PS4::removePairedDevices()
{
  uint8_t pairedDeviceBtAddr[20][6];
  int count = esp_bt_gap_get_bond_device_num();
  esp_bt_gap_get_bond_device_list(&count, pairedDeviceBtAddr);
  for (int i = 0; i < count; i++)
  {
    esp_bt_gap_remove_bond_device(pairedDeviceBtAddr[i]);
  }
}

void IoHwAb_PS4::printDeviceAddress()
{
  const uint8_t *point = esp_bt_dev_get_address();
  for (int i = 0; i < 6; i++)
  {
    char str[3];
    sprintf(str, "%02x", (unsigned int)point[i]);
    Serial.print(str);
    if (i < 5)
    {
      Serial.print(":");
    }
  }
  Serial.println();
}

String IoHwAb_PS4::getMacAdress()
{
  const uint8_t *mac = esp_bt_dev_get_address();
  char buf[18];
  sprintf(buf, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(buf);
}

bool IoHwAb_PS4::isConnected()
{
  return PS4.isConnected();
}
