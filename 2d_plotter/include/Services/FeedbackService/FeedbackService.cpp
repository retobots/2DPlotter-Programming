#include "FeedbackService.h"

FeedbackService::FeedbackService() {}

FeedbackService &FeedbackService::getInstance()
{
  static FeedbackService instance;
  return instance;
}

void FeedbackService::setup()
{
  Serial.println("[SET UP]: Feedback Service Done!");
}

int FeedbackService::calculateTotalLines(File &file)
{
  // Check file
  if (!file)
  {
    Serial.println("Cannot open file!");
    return 0;
  }

  int lineCount = 0;

  while (file.available())
  {
    // Đọc tới dấu xuống dòng
    String line = file.readStringUntil('\n');

    // Xoá khoảng trắng đầu/cuối, gồm cả '\r', ' ', '\t'
    line.trim();

    // Nếu dòng trống (chỉ toàn whitespace) thì bỏ qua
    if (line.length() == 0)
    {
      continue;
    }

    // Dòng còn lại được coi là "có nội dung G-code"
    lineCount++;
  }

  file.close(); // Giữ nguyên hành vi cũ

  Serial.print("Total NON-EMPTY lines in file: ");
  Serial.println(lineCount);
  totalLines = lineCount;
  Serial.println("[PROCESS]: Total lines (non-empty) calculated: " + String(totalLines));
  return totalLines;
}

int FeedbackService::calculateTotalLines(const String &filename)
{
  File file = SD.open(filename.c_str());
  if (!file)
  {
    Serial.println("Cannot open file for counting lines!");
    totalLines = 0;
    return 0;
  }

  // Tái sử dụng hàm cũ – hàm này sẽ tự close(file)
  return calculateTotalLines(file);
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

int &FeedbackService::getTotalLines()
{
  return totalLines;
}

int &FeedbackService::getCurrentLine()
{
  return currentLine;
}
