#pragma once

// Tốc độ stepper (số bước/giây)
#define DEFAULT_SPEED_X   1000
#define DEFAULT_SPEED_Y   1000

// Độ phân giải: mm/step hoặc steps/mm tùy cách tính
#define STEPS_PER_MM_X    80.0
#define STEPS_PER_MM_Y    80.0

// Góc servo nhấc/hạ bút
#define PEN_DOWN_ANGLE    30
#define PEN_UP_ANGLE      90

// Thời gian delay sau mỗi bước
#define STEP_DELAY_US     500

// Độ nhạy joystick
#define JOYSTICK_DEADZONE 10

// Tốc độ tối đa di chuyển khi điều khiển tay
#define MAX_MANUAL_SPEED  5.0  // mm/s
