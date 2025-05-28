#pragma once

// Giới hạn toạ độ làm việc (theo mm)
#define X_MIN  0.0
#define X_MAX  200.0
#define Y_MIN  0.0
#define Y_MAX  200.0

// Vùng an toàn (có thể thêm nếu cần)
#define SAFE_ZONE_MARGIN  5.0

// Ngưỡng giới hạn để cảnh báo buzzer
#define EDGE_WARNING_THRESHOLD 1.0  // mm gần biên sẽ cảnh báo
