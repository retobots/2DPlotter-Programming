#include "IoHwAb_RTC.h"

IoHwAb_RTC::IoHwAb_RTC()
{
}

IoHwAb_RTC &IoHwAb_RTC::getInstance()
{
  static IoHwAb_RTC instance;
  return instance;
}

void IoHwAb_RTC::setup()
{
  Serial.println("[SET UP]: RTC Starting Setup!");

  Wire.begin(RTC_SDA_PIN, RTC_SCL_PIN);

  if (!rtc.begin())
  {
    Serial.println("[ERROR]: RTC not found!");
    while (1)
      ; // Dừng chương trình nếu lỗi
  }

  if (rtc.lostPower())
  {
    Serial.println("[RTC]: Lost power, setting default time");
    rtc.adjust(DateTime(2025, 7, 27, 10, 30, 0)); // Năm, Tháng, Ngày, Giờ, Phút, Giây
  }

  Serial.println("[SET UP]: RTC Done!");
}

String IoHwAb_RTC::getElapsedTime()
{
  if (!isStart)
    return "00:00:00";

  DateTime now = rtc.now();
  TimeSpan elapsed = now - startTime;

  int hours = elapsed.hours() + elapsed.days() * 24; // Tổng giờ kể cả qua ngày
  int minutes = elapsed.minutes();
  int seconds = elapsed.seconds();

  char buffer[16];
  sprintf(buffer, "%d:%02d:%02d", hours, minutes, seconds);
  return String(buffer);
}

void IoHwAb_RTC::startTimer()
{
  if (!isStart)
  {
    startTime = rtc.now();
    isStart = true;
  }
}

void IoHwAb_RTC::changeStatus()
{
  isStart = !isStart;
}