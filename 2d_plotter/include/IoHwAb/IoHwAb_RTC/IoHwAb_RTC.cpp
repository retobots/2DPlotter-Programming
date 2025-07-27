#include "IoHwAb_RTC.h"

IoHwAb_RTC::IoHwAb_RTC() : rtc(RTC_SDA_PIN, RTC_SCL_PIN)
{
}

IoHwAb_RTC &IoHwAb_RTC::getInstance()
{
  static IoHwAb_RTC instance;
  return instance;
}

void IoHwAb_RTC::setup()
{
  // Khởi động DS1307
  rtc.begin();
  rtc.setDate(27, 7, 2025);
  rtc.setTime(10, 30, 0);
  rtc.setDOW(SUNDAY);
}

String IoHwAb_RTC::getElapsedTime()
{
  Time now = rtc.getTime();

  // Tính số giây đã trôi qua kể từ startTimer()
  long elapsed = rtc.getUnixTime(now) - rtc.getUnixTime(startTime);

  int hours = elapsed / 3600;
  int minutes = (elapsed % 3600) / 60;
  int seconds = elapsed % 60;

  // Định dạng chuỗi: HH:MM:SS (có thể > 24h)
  char buffer[16];
  sprintf(buffer, "%d:%02d:%02d", hours, minutes, seconds);
  return String(buffer);
}

void IoHwAb_RTC::startTimer()
{
  if (!isStart)
  {
    startTime = rtc.getTime();
    isStart = true; // Đánh dấu đã bắt đầu
  }
}

void IoHwAb_RTC::changeStatus()
{
  isStart = !isStart; // Đảo trạng thái
}