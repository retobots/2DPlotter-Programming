#include "FeedbackService.h"

FeedbackService::FeedbackService() {}

FeedbackService &FeedbackService::getInstance()
{
  static FeedbackService instance;
  return instance;
}

void FeedbackService::setup()
{
  // Khởi tạo LCD và buzzer
}

int FeedbackService::calculateTotalLines(File &file)
{
  // Đọc file và tính tổng số dòng
  if (!file)
  {
    Serial.println("Cannot open file!");
    return 0; // Trả về 0 nếu không mở được file
  }

  int lineCount = 0;
  while (file.available())
  {
    String line = file.readStringUntil('\n');
    lineCount++;
  }
  file.close();
  Serial.print("Total lines in file: ");
  Serial.println(lineCount);
  totalLines = lineCount; // Lưu tổng số dòng
  return totalLines;
}

int FeedbackService::calculatePercentage(int currentLine, int totalLines)
{
  if (totalLines == 0)
  {
    return 0;
  }
  percentage = (currentLine * 100) / totalLines;

  return percentage;
}

int FeedbackService::getPercentage()
{
  return percentage;
}