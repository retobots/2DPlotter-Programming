/*======================================================== [INCLUDE LIBRARY] ===================================================*/

#include "JoystickHAL.hpp"

/*=====================================================  [FUNCTION DEFINITIONS] ================================================*/

/*========================================================== ( Hàm Setup ) =====================================================*/

void setupJoystick()
{
    pinmode(SW, INPUT_PULLUP);
}

/*======================================================= ( Hàm Lấy Tọa Độ ) ====================================================*/

Point ordinateReader()
{
    Point points = {0, 0};

    int deltaX = map(analogRead(A0), 0, 1023, -5, 5);
    int deltaY = map(analogRead(A1), 0, 1023, -5, 5);

    // Deadzone
    if (abs(deltaX) < 1)
        deltaX = 0;
    if (abs(deltaY) < 1)
        deltaY = 0;

    // Cập nhật tọa độ thực
    xPos += deltaX;
    yPos += deltaY;

    points.x = xPos;
    points.y = yPos;

    return points;
}

/*======================================================= ( Hàm Đọc Nút Nhấn ) =================================================*/

bool isPressed()
{
    return (digitalRead(SW) == LOW ? true : false);
}