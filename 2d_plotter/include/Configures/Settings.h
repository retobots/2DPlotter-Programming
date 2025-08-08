#pragma once

typedef struct point
{
    float x;
    float y;
} point;

/*========== SERVO SETTING ==========*/

// Góc servo nhấc/hạ bút
#define PEN_DOWN_ANGLE 90
#define PEN_UP_ANGLE 30

/*=========== STEPPER SETTING ==========*/

// Tốc độ stepper (số bước/giây)
#define DEFAULT_SPEED_X 1000
#define DEFAULT_SPEED_Y 1000

// Độ phân giải: mm/step hoặc steps/mm tùy cách tính
#define STEPS_PER_MM_X 80.0
#define STEPS_PER_MM_Y 80.0

// Giới hạn toạ độ làm việc (theo mm)
#define X_MIN 0.0
#define X_MAX 400.0
#define Y_MIN 0.0
#define Y_MAX 400.0

// Thời gian delay sau mỗi bước
#define STEP_DELAY_US 500

// Độ nhạy joystick
#define JOYSTICK_DEADZONE 10

// Tốc độ tối đa cho các lệnh vị trí (moveTo)
// Max positional speed (steps/s). Keep high enough so it won't bottleneck runSpeed.
#define MAX_SPEED 200000

// Tốc độ tối đa riêng cho chế độ lăn tay (steps/s)
#define ROLL_MAX_SPEED 200000

// Gia tốc
#define ACCELERATION 500

/*========== PS4 SETTING ==========*/

// Tốc độ tối đa di chuyển khi điều khiển tay (mm/s base)
#define MAX_MANUAL_SPEED 2.0 // mm/s

// Vùng an toàn
#define SAFE_ZONE_MARGIN 15.0

// Ngưỡng giới hạn để cảnh báo buzzer
#define EDGE_WARNING_THRESHOLD 1.0 // mm gần biên sẽ cảnh báo
