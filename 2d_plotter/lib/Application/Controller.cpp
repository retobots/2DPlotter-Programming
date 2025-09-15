#include "Controller.hpp"

// Khởi tạo các service và phần cứng
void Controller::init() {
    Serial.begin(115200);
    
    // // Khởi tạo tầng phản hồi & hiển thị
    // FeedbackService::init();
    // MenuService::init();

    // // Có thể khởi tạo các phần HAL bên trong service
    // ManualModeController::init();  // Chuẩn bị joystick, stepper, servo
    // AutoModeController::init();    // Chuẩn bị SDCard, parser, LCD

    // FeedbackService::beepStartup(); // 1 beep báo đã sẵn sàng
}

// Vòng lặp chính
void Controller::run() {
    // // Nếu chưa chọn mode, hiển thị menu
    // if (currentMode == Mode::None) {
    //     MenuService::update();

    //     if (MenuService::isManualSelected()) {
    //         currentMode = Mode::Manual;
    //         FeedbackService::showMessage("Manual Mode");
    //     } else if (MenuService::isAutoSelected()) {
    //         currentMode = Mode::Auto;
    //         FeedbackService::showMessage("Auto Mode");
    //     }
    // }

    // // Gọi controller tương ứng với mode
    // switch (currentMode) {
    //     case Mode::Manual:
    //         ManualModeController::run();
    //         break;
    //     case Mode::Auto:
    //         AutoModeController::run();
    //         break;
    //     default:
    //         // Không làm gì cả
    //         break;
    // }
}
