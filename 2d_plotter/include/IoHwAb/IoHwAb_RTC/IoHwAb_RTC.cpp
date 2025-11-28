/*************************************************************************************************************************
 * @file    IoHwAb_RTC.cpp
 * @brief   Implementation of RTC hardware abstraction layer
 * @version 1.0
 * @date    2025-06-19
 * @author  Do Duc Nghia
 ************************************************************************************************************************/

#include "IoHwAb_RTC.h"

/*************************************************************************************************************************
 * @brief  Constructor for IoHwAb_RTC class
 ************************************************************************************************************************/
IoHwAb_RTC::IoHwAb_RTC()
{
}

/*************************************************************************************************************************
 * @brief  Get the singleton instance of IoHwAb_RTC
 *
 * @return Reference to the singleton instance
 ************************************************************************************************************************/
IoHwAb_RTC &IoHwAb_RTC::getInstance()
{
  static IoHwAb_RTC instance;
  return instance;
}

/*************************************************************************************************************************
 * @brief  Setup the RTC module
 ************************************************************************************************************************/
void IoHwAb_RTC::setup()
{
  Serial.println("[SET UP]: RTC Starting Setup!");

  Wire.begin(RTC_SDA_PIN, RTC_SCL_PIN);

  if (!rtc.begin())
  {
    Serial.println("[ERROR]: RTC not found!");
    while (1)
      ;
  }

  if (rtc.lostPower())
  {
    Serial.println("[RTC]: Lost power, setting default time");
    rtc.adjust(DateTime(2025, 7, 27, 10, 30, 0)); // Năm, Tháng, Ngày, Giờ, Phút, Giây
  }

  Serial.println("[SET UP]: RTC Done!");
}

/*************************************************************************************************************************
 * @brief  Get the elapsed time since the timer started
 *
 * @return Elapsed time in "HH:MM:SS" format
 ************************************************************************************************************************/
String IoHwAb_RTC::getElapsedTime()
{
  if (!isStart)
    return "00:00:00";

  DateTime now = rtc.now();
  TimeSpan elapsed = now - startTime;

  int hours = elapsed.hours() + elapsed.days() * 24;
  int minutes = elapsed.minutes();
  int seconds = elapsed.seconds();

  char buffer[16];
  sprintf(buffer, "%d:%02d:%02d", hours, minutes, seconds);
  return String(buffer);
}

/*************************************************************************************************************************
 * @brief  Start the timer
 ************************************************************************************************************************/
void IoHwAb_RTC::startTimer()
{
  if (!isStart)
  {
    startTime = rtc.now();
    isStart = true;
  }
}

/*************************************************************************************************************************
 * @brief  Change the timer status (start/stop)
 ************************************************************************************************************************/
void IoHwAb_RTC::changeStatus()
{
  isStart = !isStart;
}

/*************************************************************************************************************************
 * @brief  Reset the timer
 ************************************************************************************************************************/
void IoHwAb_RTC::resetTimer()
{
  isStart = false;
}