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
  int &getTotalLines();
  int &getCurrentLine();

private:
  FeedbackService();

  int totalLines = 0;  // Total lines in the file
  int currentLine = 0; // Current line being processed
  int percentage = 0;  // Percentage of completion
};
