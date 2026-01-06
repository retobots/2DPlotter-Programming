#include "AutoModeController.h"

AutoModeController::AutoModeController()
{
}

AutoModeController &AutoModeController::getInstance()
{
  static AutoModeController instance;
  return instance;
}

void AutoModeController::setup()
{
  IoHwAb_SD::getInstance().setup();
  IoHwAb_Servo::getInstance().setup();
  GcodeParserService::getInstance().setup();
  MotionControlService::getInstance().setup();

  // Set data
  data.x = X_MIN;
  data.y = Y_MIN;

  // Logging
  Serial.println("[SET UP]: AutoMode Controller Done!");
}

void AutoModeController::readSerial(point &actualPoint)
{
  static char c;

  while (Serial.available() > 0)
  {
    c = Serial.read();

    // GRBL realtime commands
    if (c == '!')
    {
      Serial.println("ok");
      continue;
    }
    if (c == '~')
    {
      Serial.println("ok");
      continue;
    }
    if (c == 0x18)
    {
      Serial.println("ok");
      lineIndex = 0;
      lineIsComment = false;
      lineSemiColon = false;
      continue;
    }

    // End of line
    if ((c == '\n') || (c == '\r'))
    {
      if (lineIndex > 0)
      {
        line[lineIndex] = '\0';

        // Handle some GRBL queries
        if (strcmp(line, "?") == 0)
        {
          Serial.print("<Idle|MPos:");
          Serial.print(actualPoint.x, 3);
          Serial.print(",");
          Serial.print(actualPoint.y, 3);
          Serial.print(",");
          Serial.print(0.000, 3);
          Serial.println("|FS:0,0>");
          lineIndex = 0;

          // ✅ reset comment flags
          lineIsComment = false;
          lineSemiColon = false;
          return;
        }

        if (strcmp(line, "$I") == 0)
        {
          Serial.println("[VER:1.1h.20210101:]");
          Serial.println("ok");
          lineIndex = 0;

          lineIsComment = false;
          lineSemiColon = false;
          return;
        }

        if (strcmp(line, "$$") == 0)
        {
          Serial.println("$0=10");
          Serial.println("$1=25");
          Serial.println("$2=0");
          Serial.println("$3=0");
          Serial.println("$4=0");
          Serial.println("$5=0");
          Serial.println("$6=0");
          Serial.println("$10=1");
          Serial.println("$11=0.010");
          Serial.println("$12=0.002");
          Serial.println("$13=0");
          Serial.println("$20=0");
          Serial.println("$21=0");
          Serial.println("$22=0");
          Serial.println("$23=0");
          Serial.println("$24=25.0");
          Serial.println("$25=500.0");
          Serial.println("$26=250");
          Serial.println("$27=1.000");
          Serial.println("$30=1000");
          Serial.println("$31=0");
          Serial.println("$32=0");
          Serial.println("$100=250.0");
          Serial.println("$101=250.0");
          Serial.println("$102=250.0");
          Serial.println("$110=500.0");
          Serial.println("$111=500.0");
          Serial.println("$112=500.0");
          Serial.println("$120=10.0");
          Serial.println("$121=10.0");
          Serial.println("$122=10.0");
          Serial.println("$130=200.0");
          Serial.println("$131=200.0");
          Serial.println("$132=200.0");
          Serial.println("ok");
          lineIndex = 0;

          lineIsComment = false;
          lineSemiColon = false;
          return;
        }

        if (strcmp(line, "$G") == 0)
        {
          Serial.println("[G90 G21 G17 G94 G54]");
          Serial.println("ok");
          lineIndex = 0;

          lineIsComment = false;
          lineSemiColon = false;
          return;
        }

        if (strcmp(line, "$N") == 0)
        {
          Serial.println("$N0=");
          Serial.println("$N1=");
          Serial.println("ok");
          lineIndex = 0;

          lineIsComment = false;
          lineSemiColon = false;
          return;
        }

        if (strcmp(line, "$X") == 0)
        {
          Serial.println("[Unlocked]");
          Serial.println("ok");
          lineIndex = 0;

          lineIsComment = false;
          lineSemiColon = false;
          return;
        }

        if (strcmp(line, "$#") == 0)
        {
          Serial.println("[G54:-1.000,-1.000,0.000]");
          Serial.println("ok");
          lineIndex = 0;

          lineIsComment = false;
          lineSemiColon = false;
          return;
        }

        if (strcmp(line, "$H") == 0)
        {
          Serial.println("ok");
          lineIndex = 0;

          lineIsComment = false;
          lineSemiColon = false;
          return;
        }

        // Normal G-code
        if (verbose)
        {
          Serial.print("Received: ");
          Serial.println(line);
        }

        GcodeParserService::getInstance().processIncomingLine(line, lineIndex, actualPoint);
        lineIndex = 0;
      }
      else
      {
        lineIndex = 0;
      }

      // ✅ ALWAYS reset comment flags at end-of-line
      lineIsComment = false;
      lineSemiColon = false;
    }
    else
    {
      // Skip comment content
      if (lineIsComment || lineSemiColon)
      {
        if (c == ')')
          lineIsComment = false;
        continue;
      }

      if (c <= ' ')
      {
        // Skip whitespace
      }
      else if (c == '/')
      {
        // Skip block delete
      }
      else if (c == '(')
      {
        lineIsComment = true;
      }
      else if (c == ';')
      {
        lineSemiColon = true;
      }
      else if (lineIndex >= LINE_BUFFER_LENGTH - 1)
      {
        Serial.println("ERROR - lineBuffer overflow");
        lineIsComment = false;
        lineSemiColon = false;
        lineIndex = 0;
      }
      else if (c >= 'a' && c <= 'z')
      {
        line[lineIndex++] = c - 'a' + 'A';
      }
      else
      {
        line[lineIndex++] = c;
      }
    }
  }
}

void AutoModeController::getGcodeFile(const String &filename)
{
  currentFilename = filename;

  gcodeFile = IoHwAb_SD::getInstance().getFile(filename);
  Serial.print("Opened G-code file: ");
  Serial.println(filename);

  if (!gcodeFile)
  {
    Serial.println("[ERROR]: Failed to open G-code file.");
    active = false;
    paused = false;
    cancelled = true;
    statusFlag = 0;
    return;
  }

  // Tính totalLines bằng một file tạm riêng, không đụng đến gcodeFile
  FeedbackService::getInstance().calculateTotalLines(currentFilename);
  FeedbackService::getInstance().getCurrentLine() = 0;
  percentage = 0;

  // Prepare internal state for a fresh run
  isInitialized = false;
  active = true;
  paused = false;
  cancelled = false;
  statusFlag = 0;

  lineIndex = 0;
  lineIsComment = false;
  lineSemiColon = false;
  isLineFull = false;

  // 2 biến này sẽ không dùng nữa trong chế độ SD
  lineVectorIndex = 0;
  charVectorIndex = 0;

  // Reset current XY reference to machine minima for a clean start
  data.x = X_MIN;
  data.y = Y_MIN;
  MotionControlService::getInstance().updateData(data);
}

void AutoModeController::readFile(File &file, point &actualPoint,
                                  int workingFlag, int &percentage,
                                  String &time, int &statusFlag)
{
  // Respect control flags: do nothing if paused, inactive, or cancelled
  if (paused || !active || cancelled)
  {
    return;
  }

  // Only abort when SD buffered contents are empty
  if (!file)
  {
    Serial.println("[PROCESS]: File handle invalid.");
    active = false;
    statusFlag = 0;
    return;
  }

  // Initialize at the beginning of a new file processing session
  if (!isInitialized)
  {
    // Reset parsing indices and flags
    lineIndex = 0;
    lineIsComment = false;
    lineSemiColon = false;
    isLineFull = false;
    lineVectorIndex = 0;
    charVectorIndex = 0;

    // Recalc total lines and reset progress
    FeedbackService::getInstance().getCurrentLine() = 1;
    percentage = 0;

    // Start timer
    isInitialized = true;
  }

  statusFlag = 1;     // Đặt cờ trạng thái đang đọc file
  isLineFull = false; // Reset trạng thái dòng đầy

  while (!isLineFull)
  {
    if (!file.available())
    {
      // Nếu còn dở một dòng chưa flush
      if (lineIndex > 0)
      {
        line[lineIndex] = '\0';

        Serial.print("G-code: ");
        Serial.println(line);

        GcodeParserService::getInstance().processIncomingLine(line, lineIndex, actualPoint);

        // Cập nhật tiến độ
        percentage = FeedbackService::getInstance().calculatePercentage(
            FeedbackService::getInstance().getCurrentLine(),
            FeedbackService::getInstance().getTotalLines());

        Serial.printf("[DEBUG]: Current line: %d, Total lines: %d, Percentage: %d%%\n",
                      FeedbackService::getInstance().getCurrentLine(),
                      FeedbackService::getInstance().getTotalLines(),
                      percentage);

        FeedbackService::getInstance().getCurrentLine()++;
        lineIndex = 0;
      }

      // ✅ FIX: reset comment flags khi kết thúc file (tránh state dính)
      lineIsComment = false;
      lineSemiColon = false;

      // Đã đọc hết file → dừng hẳn
      active = false;
      statusFlag = 0;
      Serial.println("[PROCESS]: Reached end of G-code file.");
      file.close();
      return;
    }

    c = file.read();

    // Kết thúc dòng
    if ((c == '\n') || (c == '\r'))
    {
      if (lineIndex > 0)
      {
        line[lineIndex] = '\0'; // Kết thúc chuỗi

        // In ra dòng lệnh đang xử lý (tuỳ chọn)
        Serial.print("G-code: ");
        Serial.println(line);

        isLineFull = true; // Đánh dấu đã đầy dòng

        // Xử lý dòng lệnh G-code thực tế
        GcodeParserService::getInstance().processIncomingLine(line, lineIndex, actualPoint);
        percentage = FeedbackService::getInstance().calculatePercentage(
            FeedbackService::getInstance().getCurrentLine(),
            FeedbackService::getInstance().getTotalLines());

        // Debug percentage calculation
        Serial.printf("[DEBUG]: Current line: %d, Total lines: %d, Percentage: %d%%\n",
                      FeedbackService::getInstance().getCurrentLine(),
                      FeedbackService::getInstance().getTotalLines(),
                      percentage);

        Serial.println("ok");

        // Reset buffer
        lineIndex = 0;
      }
      else
      {
        // Dòng trống hoặc chỉ comment → bỏ qua
        lineIsComment = false;
        lineSemiColon = false;
      }

      // ✅ FIX QUAN TRỌNG: luôn reset cờ comment sau khi kết thúc dòng
      lineIsComment = false;
      lineSemiColon = false;

      FeedbackService::getInstance().getCurrentLine()++; // Increment FeedbackService's currentLine
    }
    else
    {
      // Xử lý các ký tự đang đọc
      if (lineIsComment || lineSemiColon)
      {
        if (c == ')')
          lineIsComment = false;
      }
      else
      {
        if (c <= ' ')
        {
          // Bỏ qua whitespace
        }
        else if (c == '/')
        {
          // Bỏ qua block delete
        }
        else if (c == '(')
        {
          lineIsComment = true;
        }
        else if (c == ';')
        {
          lineSemiColon = true;
        }
        else if (lineIndex >= LINE_BUFFER_LENGTH - 1)
        {
          Serial.println("ERROR - lineBuffer overflow");
          lineIndex = 0;
          lineIsComment = false;
          lineSemiColon = false;
        }
        else if (c >= 'a' && c <= 'z')
        {
          line[lineIndex++] = c - 'a' + 'A'; // Viết hoa
        }
        else
        {
          line[lineIndex++] = c;
        }
      }
    }
  }
}

void AutoModeController::cancelSD()
{
  if (gcodeFile)
  {
    gcodeFile.close();
  }
  // Set flags so processing stops and a new selection starts fresh
  active = false;
  paused = false;
  cancelled = true;

  // Reset XY reference to origin so next run starts clean
  data.x = X_MIN;
  data.y = Y_MIN;
  MotionControlService::getInstance().updateData(data);
  Serial.println("Canceled reading G-code file.");
}

void AutoModeController::pauseSD()
{
  // Pause processing; keep indices so we can resume
  active = false;
  paused = true;
}

void AutoModeController::resetSD()
{
  // Reset internal parser state (avoid local variable shadowing)
  lineIndex = 0;
  lineIsComment = false;
  lineSemiColon = false;
  verbose = false;
  memset(line, 0, sizeof(line)); // Xoá bộ đệm dòng

  // Reset vector indices and flags
  lineVectorIndex = 0;
  charVectorIndex = 0;
  isLineFull = false;

  // Reset control flags
  active = false;
  paused = false;
  cancelled = false;
  isInitialized = false;

  // Reset FeedbackService's currentLine and percentage
  FeedbackService::getInstance().getCurrentLine() = 0;
  percentage = 0;

  // Reset XY reference to origin to avoid stale coordinates on next run
  data.x = X_MIN;
  data.y = Y_MIN;
  MotionControlService::getInstance().updateData(data);

  Serial.println("SD reset done.");
}

void AutoModeController::resetSerial()
{
  lineIndex = 0;
  lineIsComment = false;
  lineSemiColon = false;
  verbose = false;
  memset(line, 0, sizeof(line)); // Xoá bộ đệm dòng
  Serial.println("Serial reset done.");
}

void AutoModeController::runSerial()
{
  readSerial(data);
}

void AutoModeController::continueSD()
{
  // Allow resume even if underlying File object is closed; we stream from the buffered vector
  if (paused == false && active == true)
  {
    Serial.println("Already reading G-code file.");
    return;
  }

  active = true;
  paused = false;
  cancelled = false;

  Serial.println("Continuing reading G-code file...");
}

void AutoModeController::runSD(int workingFlag)
{
  // If paused/inactive/cancelled, do nothing (UI remains on status screen)
  if (paused || !active || cancelled)
  {
    return;
  }

  // Process one line/frame from the prepared G-code file
  readFile(gcodeFile, data, workingFlag, percentage, time, statusFlag);
}

int AutoModeController::getPercentage() const
{
  return percentage;
}

String AutoModeController::getTime() const
{
  return time;
}

int AutoModeController::getStatusFlag() const
{
  return statusFlag;
}

void AutoModeController::resetData()
{
  data.x = 0.0;
  data.y = 0.0;
}