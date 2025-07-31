#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <cstdlib>

struct point
{
    float x = 0.0;
    float y = 0.0;
};

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

int main()
{
    point actualPos;
    std::string lineInput;

    // Giả lập đọc dòng lệnh G-code
    std::cout << "Enter G-code line: ";
    std::getline(std::cin, lineInput);

    // Xử lý dòng lệnh
    processIncomingLine(lineInput, actualPos);

    // In ra vị trí hiện tại
    std::cout << "Current Position: X=" << actualPos.x << " Y=" << actualPos.y << "\n";

    return 0;
}