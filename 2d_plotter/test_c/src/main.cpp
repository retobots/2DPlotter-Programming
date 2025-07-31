#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <cstdlib>

#define LINE_BUFFER_LENGTH 512

struct point
{
    float x = 0.0;
    float y = 0.0;
};

int lineIndex = 0;
bool lineIsComment = false;
bool lineSemiColon = false;
bool verbose = false;
char line[LINE_BUFFER_LENGTH];
char c;

point data;

void processIncomingLine(const std::string &lineInput, point &actualPos)
{
    std::cout << "[PROCESS]: Processing Incoming Line\n";
    if (lineInput.empty() || lineInput == "?")
        return;

    size_t currentIndex = 0;
    std::string buffer;
    int gcode = -1;
    point newPos = actualPos;
    float zValue = 5.0;
    float iValue = 0.0, jValue = 0.0;
    float feedrate = 0.0;
    bool hasX = false, hasY = false, hasZ = false, hasI = false, hasJ = false;

    while (currentIndex < lineInput.size())
    {
        char cmd = lineInput[currentIndex++];

        // Bỏ qua khoảng trắng và xuống dòng
        if (cmd == ' ' || cmd == '\n' || cmd == '\r')
            continue;

        buffer.clear();
        char sign = '+';

        if (lineInput[currentIndex] == '-' || lineInput[currentIndex] == '+')
            sign = lineInput[currentIndex++];

        while ((isdigit(lineInput[currentIndex]) || lineInput[currentIndex] == '.') && currentIndex < lineInput.size())
            buffer += lineInput[currentIndex++];

        float value = std::atof(buffer.c_str());
        if (sign == '-')
            value = -value;

        switch (cmd)
        {
        case 'G':
            gcode = static_cast<int>(value);
            break;
        case 'X':
            newPos.x = value;
            hasX = true;
            break;
        case 'Y':
            newPos.y = value;
            hasY = true;
            break;
        case 'Z':
            zValue = value;
            hasZ = true;
            break;
        case 'I':
            iValue = value;
            hasI = true;
            break;
        case 'J':
            jValue = value;
            hasJ = true;
            break;
        case 'F':
            feedrate = value;
            break;
        case 'M':
        {
            int mcode = static_cast<int>(value);
            switch (mcode)
            {
            case 3:
                std::cout << "M3 → Pen Down\n";
                break;
            case 5:
                std::cout << "M5 → Pen Up\n";
                break;
            case 2:
                std::cout << "M2 → Program End\n";
                break;
            case 114:
                std::cout << "Position: X=" << actualPos.x
                          << " Y=" << actualPos.y
                          << " Z=" << 0.000 << "\n";
                break;
            default:
                std::cout << "Unsupported M-code: M" << mcode << "\n";
                break;
            }
            break;
        }
        default:
            break;
        }
    }

    // Giả lập điều khiển bút
    static bool penDown = false;
    if (hasZ)
    {
        if (zValue < 0 && !penDown)
        {
            std::cout << "Pen Down\n";
            penDown = true;
        }
        else if (zValue > 0 && penDown)
        {
            std::cout << "Pen Up\n";
            penDown = false;
        }
    }

    // Xử lý G-code
    switch (gcode)
    {
    case 0:
        std::cout << "Move to X=" << newPos.x << " Y=" << newPos.y << "\n";
        break;
    case 1:
        std::cout << "Draw line to X=" << newPos.x << " Y=" << newPos.y << "\n";
        break;
    case 2:
        if (hasI && hasJ)
            std::cout << "Draw arc CW to X=" << newPos.x << " Y=" << newPos.y
                      << " I=" << iValue << " J=" << jValue << "\n";
        else
            std::cout << "Warning: G02 missing I/J params\n";
        break;
    case 3:
        if (hasI && hasJ)
            std::cout << "Draw arc CCW to X=" << newPos.x << " Y=" << newPos.y
                      << " I=" << iValue << " J=" << jValue << "\n";
        else
            std::cout << "Warning: G03 missing I/J params\n";
        break;
    case 21:
        std::cout << "Set units to mm\n";
        break;
    case 90:
        std::cout << "Set absolute mode (G90)\n";
        break;
    case 91:
        std::cout << "Set relative mode (G91)\n";
        break;
    default:
        if (gcode != -1)
            std::cout << "Unsupported G-code: G" << gcode << "\n";
        break;
    }

    if (hasX || hasY)
        actualPos = newPos;

    std::cout << "ok\n";
}

void readSerial(const std::string &inputLine, point &actualPoint)
{
    for (char c : inputLine)
    {
        // === Ký tự thời gian thực GRBL ===
        if (c == '!')
        {
            std::cout << "ok\n";
            continue;
        }
        if (c == '~')
        {
            std::cout << "ok\n";
            continue;
        }
        if (c == 0x18)
        {
            std::cout << "ok\n";
            lineIndex = 0;
            lineIsComment = false;
            lineSemiColon = false;
            continue;
        }

        // === Kết thúc dòng lệnh ===
        if (c == '\n' || c == '\r')
        {
            if (lineIndex > 0)
            {
                line[lineIndex] = '\0'; // Kết thúc chuỗi C-style

                std::string cmdStr(line);

                std::cout << "Lệnh nhận được: " << cmdStr << "\n";

                if (cmdStr == "?")
                {
                    std::cout << "<Idle|MPos:" << actualPoint.x << "," << actualPoint.y << ",0.000|FS:0,0>\n";
                    lineIndex = 0;
                    continue;
                }
                if (cmdStr == "$I")
                {
                    std::cout << "[VER:1.1h.20210101:]\nok\n";
                    lineIndex = 0;
                    continue;
                }
                if (cmdStr == "$$")
                {
                    std::cout << "$0=10\n$1=25\n$2=0\n$3=0\n$4=0\n$5=0\n$6=0\n";
                    std::cout << "$10=1\n$11=0.010\n$12=0.002\n$13=0\n$20=0\n$21=0\n";
                    std::cout << "$22=0\n$23=0\n$24=25.0\n$25=500.0\n$26=250\n";
                    std::cout << "$27=1.000\n$30=1000\n$31=0\n$32=0\n";
                    std::cout << "$100=250.0\n$101=250.0\n$102=250.0\n";
                    std::cout << "$110=500.0\n$111=500.0\n$112=500.0\n";
                    std::cout << "$120=10.0\n$121=10.0\n$122=10.0\n";
                    std::cout << "$130=200.0\n$131=200.0\n$132=200.0\nok\n";
                    lineIndex = 0;
                    continue;
                }
                if (cmdStr == "$G")
                {
                    std::cout << "[G90 G21 G17 G94 G54]\nok\n";
                    lineIndex = 0;
                    continue;
                }
                if (cmdStr == "$N")
                {
                    std::cout << "$N0=\n$N1=\nok\n";
                    lineIndex = 0;
                    continue;
                }
                if (cmdStr == "$X")
                {
                    std::cout << "[Unlocked]\nok\n";
                    lineIndex = 0;
                    continue;
                }
                if (cmdStr == "$#")
                {
                    std::cout << "[G54:-1.000,-1.000,0.000]\nok\n";
                    lineIndex = 0;
                    continue;
                }
                if (cmdStr == "$H")
                {
                    std::cout << "ok\n";
                    lineIndex = 0;
                    continue;
                }

                if (verbose)
                {
                    std::cout << "Received: " << cmdStr << "\n";
                }

                processIncomingLine(cmdStr, actualPoint);
                lineIndex = 0;
            }
            else
            {
                lineIsComment = false;
                lineSemiColon = false;
            }
        }
        else
        {
            // === Xử lý ký tự trong dòng ===
            if (lineIsComment || lineSemiColon)
            {
                if (c == ')')
                    lineIsComment = false;
            }
            else
            {
                if (c <= ' ')
                { /* bỏ qua khoảng trắng */
                }
                else if (c == '/')
                { /* bỏ block delete */
                }
                else if (c == '(')
                    lineIsComment = true;
                else if (c == ';')
                    lineSemiColon = true;
                else if (lineIndex >= LINE_BUFFER_LENGTH - 1)
                {
                    std::cout << "ERROR - lineBuffer overflow\n";
                    lineIsComment = false;
                    lineSemiColon = false;
                    lineIndex = 0;
                }
                else if (c >= 'a' && c <= 'z')
                    line[lineIndex++] = c - 'a' + 'A';
                else
                    line[lineIndex++] = c;
            }
        }
    }

    // Nếu kết thúc input mà chưa có \n nhưng vẫn có lệnh, thì xử lý luôn
    if (lineIndex > 0)
    {
        line[lineIndex] = '\0';
        std::string cmdStr(line);
        std::cout << "Incoming Line: " << cmdStr << "\n";
        processIncomingLine(cmdStr, actualPoint);
        lineIndex = 0;
    }
}

int main()
{
    point actualPos;
    std::string input;

    std::cout << "Get input gcode string:\n";
    std::getline(std::cin, input);
    readSerial(input, actualPos);

    std::cout << "Final Position: X=" << actualPos.x << " Y=" << actualPos.y << "\n";
    return 0;
}