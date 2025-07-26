#pragma once
#include <Arduino.h>
#include <SD.h>
class FeedbackService
{
public:
  static FeedbackService &getInstance();

  void setup();
  int calculateTotalLines(File &file);
  int calculatePercentage(int currentLine, int totalLines);
  int getPercentage();

private:
  FeedbackService();

  int totalLines = 0; // Total lines in the file
  int percentage = 0; // Percentage of completion
};
