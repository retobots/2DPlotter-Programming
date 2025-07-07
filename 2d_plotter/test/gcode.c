bạn thấy code này sao ?

#include "Services/GcodeParserService.h"

                      GcodeParserService::GcodeParserService()
{
}

GcodeParserService &GcodeParserService::getInstance()
{
    static GcodeParserService instance;
    return instance;
}

void GcodeParserService::setup()
{
    pinMode(2, OUTPUT);

    digitalWrite(2, HIGH); // Bật LED
    delay(500);
    digitalWrite(2, LOW); // Tắt LED
    delay(500);
}

void GcodeParserService::processIncomingLine(char *line, int charNB, point &actualPos)
{
    // Kiểm tra lệnh rỗng hoặc lệnh trạng thái GRBL
    if (line[0] == '\0' || strcmp(line, "?") == 0)
    {
        return; // Bỏ qua lệnh trống hoặc lệnh ? (đã xử lý trong readSerial)
    }

    unsigned long currentTime = millis();
    static unsigned long lastLedToggle = 0;
    static bool ledState = false;
    if (currentTime - lastLedToggle >= 1000)
    { // Giảm thời gian từ 4000ms xuống 1000ms
        ledState = !ledState;
        digitalWrite(2, ledState);
        lastLedToggle = currentTime;
    }

    int currentIndex = 0;
    char buffer[128];               // Tăng kích thước buffer để an toàn
    point newPos = {0.0, 0.0, 0.0}; // Khởi tạo vị trí mới

    while (currentIndex < charNB && line[currentIndex] != '\0')
    {
        switch (line[currentIndex++])
        {
        case 'U':
            // penUp();
            Serial.println("Pen Up");
            break;
        case 'D':
            // penDown();
            Serial.println("Pen Down");
            break;
        case 'G':
        {
            // Đọc số lệnh G (hỗ trợ 1 hoặc 2 chữ số)
            int cmdIndex = 0;
            while (currentIndex < charNB && isdigit(line[currentIndex]) && cmdIndex < 2)
            {
                buffer[cmdIndex++] = line[currentIndex++];
            }
            buffer[cmdIndex] = '\0';

            switch (atoi(buffer))
            {
            case 0: // G00
            case 1: // G01
            {
                newPos.x = actualPos.x;
                newPos.y = actualPos.y;

                // Phân tích tham số X, Y
                while (currentIndex < charNB && line[currentIndex] != '\0')
                {
                    char param = line[currentIndex++];
                    if (param == 'X' || param == 'Y')
                    {
                        cmdIndex = 0;
                        while (currentIndex < charNB && line[currentIndex] != ' ' && line[currentIndex] != '\0')
                        {
                            buffer[cmdIndex++] = line[currentIndex++];
                        }
                        buffer[cmdIndex] = '\0';
                        float value = atof(buffer);
                        if (param == 'X')
                            newPos.x = value;
                        else if (param == 'Y')
                            newPos.y = value;
                    }
                    else
                    {
                        // Bỏ qua các ký tự không phải tham số
                        while (currentIndex < charNB && line[currentIndex] != ' ' && line[currentIndex] != '\0')
                            currentIndex++;
                    }
                }

                // drawLine(newPos.x, newPos.y);
                actualPos.x = newPos.x;
                actualPos.y = newPos.y;
                Serial.println("Draw Line");
                break;
            }
            default:
                Serial.print("G command not supported: G");
                Serial.println(buffer);
                break;
            }
        }
        break;
        case 'M':
        {
            // Đọc số lệnh M (hỗ trợ 1-3 chữ số)
            int cmdIndex = 0;
            while (currentIndex < charNB && isdigit(line[currentIndex]) && cmdIndex < 3)
            {
                buffer[cmdIndex++] = line[currentIndex++];
            }
            buffer[cmdIndex] = '\0';

            switch (atoi(buffer))
            {
            case 300:
            {
                float Spos = 0.0;
                while (currentIndex < charNB && line[currentIndex] != '\0')
                {
                    if (line[currentIndex++] == 'S')
                    {
                        cmdIndex = 0;
                        while (currentIndex < charNB && line[currentIndex] != ' ' && line[currentIndex] != '\0')
                        {
                            buffer[cmdIndex++] = line[currentIndex++];
                        }
                        buffer[cmdIndex] = '\0';
                        Spos = atof(buffer);
                    }
                }
                if (Spos == 30)
                {
                    // penDown();
                    Serial.println("Pen Down");
                }
                else if (Spos == 50)
                {
                    // penUp();
                    Serial.println("Pen Up");
                }
                else
                {
                    Serial.print("Invalid S value for M300: ");
                    Serial.println(Spos);
                }
                break;
            }
            case 114: // M114 - Report position
                Serial.print("Absolute position : X = ");
                Serial.print(actualPos.x);
                Serial.print("  -  Y = ");
                Serial.println(actualPos.y);
                break;
            default:
                Serial.print("Command not recognized : M");
                Serial.println(buffer);
                break;
            }
        }
        break;
        default:
            Serial.print("Command not recognized: ");
            Serial.println(line[currentIndex - 1]);
            break;
        }
    }

    // Chỉ gửi một "ok" duy nhất sau khi xử lý xong lệnh
    Serial.println("ok");
}

void GcodeParserService::readSerial(point &actualPoint)
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
                    Serial.print(actualPoint.z, 3);
                    Serial.println("|FS:0,0>");
                    lineIndex = 0;
                    return;
                }

                if (strcmp(line, "$I") == 0)
                {
                    Serial.println("[ESP32Plotter v1.0: Custom GRBL Emulator]");
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
                processIncomingLine(line, lineIndex, actualPoint);
                Serial.println("ok");
                lineIndex = 0;
            }
            else
            {
                // Bỏ qua dòng trống hoặc comment
                lineIsComment = false;
                lineSemiColon = false;
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
                    line[lineIndex++] = c; // Lưu ký tự
                }
            }
        }
    }
}
