#ifndef IOT_MANAGER_HPP
#define IOT_MANAGER_HPP

#include "config.hpp"

void setupIoT();
void sendStatusToServer(float x, float y, bool penIsDown);
void checkIncomingIoTCommand(); // Kiểm tra lệnh mới từ MQTT hoặc HTTP

#endif