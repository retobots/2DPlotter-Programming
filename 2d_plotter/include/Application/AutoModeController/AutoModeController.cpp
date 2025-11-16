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
  static char c; // Biến lưu ký tự đọc từ Serial
  while (Serial.available() > 0)
  {
    c = Serial.read();

    // Xử lý ký tự thời gian thực của GRBL
    if (c == '!')
    { // Feed hold
      Serial.println("ok");
      continue;
    }
    if (c == '~')
    { // Cycle resume
      Serial.println("ok");
      continue;
    }
    if (c == 0x18)
    { // Soft reset
      Serial.println("ok");
      // Có thể thêm logic reset trạng thái nếu cần
      lineIndex = 0;
      lineIsComment = false;
      lineSemiColon = false;
      continue;
    }

    // Xử lý các ký tự thuộc dòng lệnh
    if ((c == '\n') || (c == '\r'))
    {
      if (lineIndex > 0)
      {
        line[lineIndex] = '\0'; // Kết thúc chuỗi

        // Xử lý các lệnh GRBL
        if (strcmp(line, "?") == 0)
        {
          Serial.print("<Idle|MPos:");
          Serial.print(actualPoint.x, 3);
          Serial.print(",");
          Serial.print(actualPoint.y, 3);
          Serial.print(",");
          Serial.print(0.000, 3); // Z = 0 nếu không dùng
          Serial.println("|FS:0,0>");
          lineIndex = 0;
          return;
        }

        if (strcmp(line, "$I") == 0)
        {
          Serial.println("[VER:1.1h.20210101:]");
          Serial.println("ok");
          lineIndex = 0;
          return;
        }

        if (strcmp(line, "$$") == 0)
        {
          // Trả về danh sách tham số GRBL
          Serial.println("$0=10");      // Step pulse time
          Serial.println("$1=25");      // Step idle delay
          Serial.println("$2=0");       // Step pulse invert
          Serial.println("$3=0");       // Direction invert
          Serial.println("$4=0");       // Step enable invert
          Serial.println("$5=0");       // Limit pins invert
          Serial.println("$6=0");       // Probe pin invert
          Serial.println("$10=1");      // Status report mask
          Serial.println("$11=0.010");  // Junction deviation
          Serial.println("$12=0.002");  // Arc tolerance
          Serial.println("$13=0");      // Report inches
          Serial.println("$20=0");      // Soft limits
          Serial.println("$21=0");      // Hard limits
          Serial.println("$22=0");      // Homing cycle
          Serial.println("$23=0");      // Homing direction
          Serial.println("$24=25.0");   // Homing feed
          Serial.println("$25=500.0");  // Homing seek
          Serial.println("$26=250");    // Homing debounce
          Serial.println("$27=1.000");  // Homing pull-off
          Serial.println("$30=1000");   // Max spindle speed
          Serial.println("$31=0");      // Min spindle speed
          Serial.println("$32=0");      // Laser mode
          Serial.println("$100=250.0"); // X steps/mm
          Serial.println("$101=250.0"); // Y steps/mm
          Serial.println("$102=250.0"); // Z steps/mm
          Serial.println("$110=500.0"); // X max rate
          Serial.println("$111=500.0"); // Y max rate
          Serial.println("$112=500.0"); // Z max rate
          Serial.println("$120=10.0");  // X acceleration
          Serial.println("$121=10.0");  // Y acceleration
          Serial.println("$122=10.0");  // Z acceleration
          Serial.println("$130=200.0"); // X max travel
          Serial.println("$131=200.0"); // Y max travel
          Serial.println("$132=200.0"); // Z max travel
          Serial.println("ok");
          lineIndex = 0;
          return;
        }

        if (strcmp(line, "$G") == 0)
        {
          Serial.println("[G90 G21 G17 G94 G54]");
          Serial.println("ok");
          lineIndex = 0;
          return;
        }

        if (strcmp(line, "$N") == 0)
        {
          Serial.println("$N0=");
          Serial.println("$N1=");
          Serial.println("ok");
          lineIndex = 0;
          return;
        }

        if (strcmp(line, "$X") == 0)
        {
          Serial.println("[Unlocked]");
          Serial.println("ok");
          lineIndex = 0;
          return;
        }

        if (strcmp(line, "$#") == 0)
        {
          Serial.println("[G54:-1.000,-1.000,0.000]");
          Serial.println("ok");
          lineIndex = 0;
          return;
        }

        if (strcmp(line, "$H") == 0)
        {
          Serial.println("ok");
          lineIndex = 0;
          return;
        }

        // Xử lý các lệnh G-code khác
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
        // Bỏ qua dòng trống hoặc comment
        lineIsComment = false;
        lineSemiColon = false;
        lineIndex = 0; // Reset lineIndex để tránh lỗi
      }
    }
    else
    {
      // Xử lý các ký tự trong dòng
      if (lineIsComment || lineSemiColon)
      {
        if (c == ')')
        {
          lineIsComment = false; // Kết thúc comment
        }
      }
      else
      {
        if (c <= ' ')
        {
          // Bỏ qua khoảng trắng và ký tự điều khiển
        }
        else if (c == '/')
        {
          // Bỏ qua block delete
        }
        else if (c == '(')
        {
          lineIsComment = true; // Bắt đầu comment
        }
        else if (c == ';')
        {
          lineSemiColon = true; // Bắt đầu comment kiểu dấu chấm phẩy
        }
        else if (lineIndex >= LINE_BUFFER_LENGTH - 1)
        {
          Serial.println("ERROR - lineBuffer overflow");
          lineIsComment = false;
          lineSemiColon = false;
          lineIndex = 0; // Reset buffer để tránh lỗi
        }
        else if (c >= 'a' && c <= 'z')
        {
          line[lineIndex++] = c - 'a' + 'A'; // Chuyển đổi thành chữ hoa
        }
        else
        {
          if (lineIndex < LINE_BUFFER_LENGTH - 1)
            line[lineIndex++] = c;
          else
          {
            Serial.println("ERROR - buffer overflow");
            lineIndex = 0;
          }
        }
      }
    }
  }
}

void AutoModeController::getGcodeFile(const String &filename)
{
  gcodeFile = IoHwAb_SD::getInstance().getFile(filename);
  Serial.print("Opened G-code file: ");
  Serial.println(filename);

  // Prepare internal state for a fresh run
  isInitialized = false;
  active = true;
  paused = false;
  cancelled = false;
  statusFlag = 0;
  percentage = 0;

  lineIndex = 0;
  lineIsComment = false;
  lineSemiColon = false;
  isLineFull = false;
  lineVectorIndex = 0;
  charVectorIndex = 0;

  // Reset current XY reference to machine minima for a clean start
  data.x = X_MIN;
  data.y = Y_MIN;
  MotionControlService::getInstance().updateData(data);

  // Reset feedback counters for a new file
  FeedbackService::getInstance().getCurrentLine() = 0;
}

void AutoModeController::readFile(File &file, point &actualPoint, int workingFlag, int &percentage, String &time, int &statusFlag)
{
  // Respect control flags: do nothing if paused, inactive, or cancelled
  if (paused || !active || cancelled)
  {
    return;
  }

  // Only abort when SD buffered contents are empty
  if (IoHwAb_SD::getInstance().isFileContentsEmpty())
  {
    Serial.println("[PROCESS]: File not available (empty contents).");
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
    FeedbackService::getInstance().calculateTotalLines(file);
    FeedbackService::getInstance().getCurrentLine() = 0;
    percentage = 0;

    // Start timer
    isInitialized = true;
  }

  statusFlag = 1;     // Đặt cờ trạng thái đang đọc file
  isLineFull = false; // Reset trạng thái dòng đầy

  while (!isLineFull)
  {
    char c = IoHwAb_SD::getInstance().getCharFromVectorLine(lineVectorIndex, charVectorIndex);

    // Kết thúc dòng
    if ((c == '\n') || (c == '\r'))
    {
      if (lineIndex > 0)
      {
        line[lineIndex] = '\0'; // Kết thúc chuỗi

        // In ra dòng lệnh đang xử lý (tuỳ chọn)
        Serial.print("G-code: ");
        Serial.println(line);

        lineVectorIndex++;
        charVectorIndex = 0;
        isLineFull = true; // Đánh dấu đã đầy dòng

        // Xử lý dòng lệnh G-code thực tế
        GcodeParserService::getInstance().processIncomingLine(line, lineIndex, actualPoint);
        percentage = FeedbackService::getInstance().calculatePercentage(FeedbackService::getInstance().getCurrentLine(), FeedbackService::getInstance().getTotalLines());

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
      charVectorIndex++;
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