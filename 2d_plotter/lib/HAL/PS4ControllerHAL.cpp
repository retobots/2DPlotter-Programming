/*======================================================== [INCLUDE LIBRARY] ===================================================*/

#include "PS4ControllerHAL.hpp"

/*=====================================================  [FUNCTION DEFINITIONS] ================================================*/

/*========================================================== ( Hàm Setup ) =====================================================*/

void setupPS4()
{
    PS4.begin(); // MAC ESP32
    Serial.println("Ready for drawing...");
}

/*======================================================= ( Hàm Lấy Tọa Độ ) ====================================================*/

PointData readPS4()
{
    PS4.update();

    PointData points;

    if (!PS4.isConnected())
        return;

    int rx = PS4.RStickX();
    int ry = PS4.RStickY();

    // Vùng chết
    if (abs(rx) < DEADZONE && abs(ry) < DEADZONE)
        return;

    // Lưu vị trí cũ
    float x_old = x;
    float y_old = y;

    // Tính vector chuẩn hóa
    float magnitude = sqrt(rx * rx + ry * ry);
    float vx = rx / magnitude;
    float vy = ry / magnitude;

    // Di chuyển một đoạn nhỏ theo hướng
    x += vx * SPEED;
    y += vy * SPEED;

    points.x_new = x;
    points.y_new = y;
    points.x_old = x_old;
    points.y_old = y_old;

    delay(10); // Điều chỉnh mượt tùy tốc độ máy

    // Trả về tọa độ
    return points;
}

/*======================================================= ( Hàm Đọc Nút Nhấn ) =================================================*/
