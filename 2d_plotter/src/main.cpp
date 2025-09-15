#include "Controller.hpp"  // lớp trung tâm của Application Layer

Controller app;  // Khởi tạo controller chính (có thể là singleton)

void setup() {
    app.init();  // Khởi tạo toàn bộ hệ thống: LCD, PS4, Stepper, v.v.
}

void loop() {
    app.run();   // Gọi luồng xử lý chính (chọn mode, điều khiển, vẽ G-code)
}
