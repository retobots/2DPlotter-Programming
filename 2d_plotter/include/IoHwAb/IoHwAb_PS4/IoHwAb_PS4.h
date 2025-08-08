#pragma once
#include <Arduino.h>
#include "PS4Controller.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "esp_err.h"
#include "Settings.h"
#include "IoHwAb_Servo.h"
#include "UIMenuService.h"

class IoHwAb_PS4
{
public:
  static IoHwAb_PS4 &getInstance();

  void setup();
  point readPS4();
  bool isButtonPressed();
  void startConnect();
  void liftPen();
  void dropPen();
  void cancel();
  void printDeviceAddress();
  void removePairedDevices();
  String getMacAdress();
  bool isConnected();
  void reconnect();

private:
  IoHwAb_PS4();
};
