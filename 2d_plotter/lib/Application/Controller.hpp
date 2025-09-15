#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <Arduino.h>

// // Các service và controller cần dùng
// #include "../Services/MenuService.hpp"
// #include "../Services/FeedbackService.hpp"
// #include "../Services/MainControl/ManualModeController.hpp"
// #include "../Services/MainControl/AutoModeController.hpp"

enum class Mode {
    None,
    Manual,
    Auto
};

class Controller {
public:
    void init();   // Khởi tạo toàn bộ hệ thống
    void run();    // Vòng lặp chính xử lý luồng theo mode

private:
    Mode currentMode = Mode::None;
};

#endif
