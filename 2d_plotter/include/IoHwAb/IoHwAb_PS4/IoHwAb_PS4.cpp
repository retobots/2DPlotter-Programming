/*************************************************************************************************************************
 * @file    IoHwAb_PS4.cpp
 * @brief   Definitions of functions declared in the header file
 * @version 1.0
 * @date    2025-06-19
 * @author  Do Duc Nghia
 ************************************************************************************************************************/

#include "IoHwAb_PS4.h"

/*************************************************************************************************************************
 * @brief Constructor
 ************************************************************************************************************************/
IoHwAb_PS4::IoHwAb_PS4()
{
}

/*************************************************************************************************************************
 * @brief Get the singleton instance
 ************************************************************************************************************************/
IoHwAb_PS4 &IoHwAb_PS4::getInstance()
{
  static IoHwAb_PS4 instance;
  return instance;
}

/*************************************************************************************************************************
 * @brief Setup the PS4 controller
 ************************************************************************************************************************/
void IoHwAb_PS4::setup()
{
  Serial.println("[SET UP]: PS4 Controller Starting Setup!");

  PS4.begin();
  // Logging
  Serial.println("[SET UP]: PS4 Controller Done!");
}

/*************************************************************************************************************************
 * @brief Start connecting to the PS4 controller
 ************************************************************************************************************************/
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

/*************************************************************************************************************************
 * @brief Lift the pen when R1 button is pressed
 ************************************************************************************************************************/
void IoHwAb_PS4::liftPen()
{
  if (PS4.R1() && PS4.isConnected())
  {
    IoHwAb_Servo::getInstance().liftPen();
    Serial.println("[PS4]: Pen lifted");
  }
}

/*************************************************************************************************************************
 * @brief Drop the pen when L1 button is pressed
 ************************************************************************************************************************/
void IoHwAb_PS4::dropPen()
{
  if (PS4.L1() && PS4.isConnected())
  {
    IoHwAb_Servo::getInstance().dropPen();
    Serial.println("[PS4]: Pen dropped");
  }
}

/*************************************************************************************************************************
 * @brief Cancel the PS4 connection when Touchpad button is pressed
 ************************************************************************************************************************/
void IoHwAb_PS4::cancel()
{
  if (PS4.Touchpad() && PS4.isConnected())
  {
    IoHwAb_Servo::getInstance().liftPen();
    Serial.println("[PS4]: Pen lifted (cancel)");
    removePairedDevices();
    Serial.println("[PS4]: Disconnected");
    IoHwAb_LCD::getInstance().clear();
    PS4.end(); // Disconnect the controller
    UIMenuService::getInstance().PS4ModeScreen(2);
  }
}

/*************************************************************************************************************************
 * @brief Read the PS4 controller joystick and convert to movement point
 *
 * @return point structure containing x and y movement values
 ************************************************************************************************************************/
point IoHwAb_PS4::readPS4()
{
  point points = {0.00, 0.00};

  if (!PS4.isConnected())
    return point{};

  int rx = PS4.RStickX();
  int ry = PS4.RStickY();

  // Dead zone
  if (abs(rx) < SAFE_ZONE_MARGIN && abs(ry) < SAFE_ZONE_MARGIN)
    return point{};

  // Calculate the normalized vector
  float magnitude = sqrt(rx * rx + ry * ry);
  float vx = rx / magnitude;
  float vy = ry / magnitude;

  // Move a small step in the direction of the joystick input
  points.x += vx * MAX_MANUAL_SPEED;
  points.y += vy * MAX_MANUAL_SPEED;

  // Removed delay(10) to avoid blocking speed tick

  // Return the coordinates
  return points;
}

/*************************************************************************************************************************
 * @brief Check if any button is pressed on the PS4 controller
 *
 * @return true if any button is pressed, false otherwise
 ************************************************************************************************************************/
bool IoHwAb_PS4::isButtonPressed()
{
  return false;
}

/*************************************************************************************************************************
 * @brief Remove all paired devices from the PS4 controller
 ************************************************************************************************************************/
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

/*************************************************************************************************************************
 * @brief Print the MAC address of the PS4 controller
 ************************************************************************************************************************/
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

/*************************************************************************************************************************
 * @brief Get the MAC address of the PS4 controller as a String
 *
 * @return MAC address in String format
 ************************************************************************************************************************/
String IoHwAb_PS4::getMacAdress()
{
  const uint8_t *mac = esp_bt_dev_get_address();
  char buf[18];
  sprintf(buf, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(buf);
}

/*************************************************************************************************************************
 * @brief Check if the PS4 controller is connected
 *
 * @return true if connected, false otherwise
 ************************************************************************************************************************/
bool IoHwAb_PS4::isConnected()
{
  return PS4.isConnected();
}

/*************************************************************************************************************************
 * @brief Reconnect to the PS4 controller if disconnected
 ************************************************************************************************************************/
void IoHwAb_PS4::reconnect()
{
  if (!PS4.isConnected())
  {
    Serial.println("[PS4]: Reconnecting...");
    PS4.begin();
  }
}
