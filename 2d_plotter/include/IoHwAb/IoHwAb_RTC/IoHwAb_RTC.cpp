#include "IoHwAb_RTC.h"

IoHwAb_RTC::IoHwAb_RTC() {}

IoHwAb_RTC &IoHwAb_RTC::getInstance()
{
  static IoHwAb_RTC instance;
  return instance;
}

void IoHwAb_RTC::setup()
{
  // Khởi động DS1307
}

std::string IoHwAb_RTC::getCurrentTime()
{
  // Trả về chuỗi thời gian hiện tại
  return "00:00";
}
