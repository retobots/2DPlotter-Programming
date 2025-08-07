#include <Arduino.h>
#include "AppController.h"
#include "IoHwAb_Servo.h"
#include "IoHwAb_Buzzer.h"

void setup()
{
    Serial2.begin(112500); // Khởi tạo Serial với tốc độ 112500 bps
    Serial.begin(115200);  // Khởi tạo Serial với tốc độ 115200 bps

    // Khởi tạo app controller
    AppController::getInstance().setup();
}

void loop()
{
    Serial.println("[PROCESS]: Running Application Loop");
    // Chạy ứng dụng chính
    AppController::getInstance().run();

    // IoHwAb_Servo::getInstance().dropPen();
    // IoHwAb_Buzzer::getInstance().beepOnce();
    // delay(5000);
    // IoHwAb_Servo::getInstance().liftPen();
    // IoHwAb_Buzzer::getInstance().beepOnce();
    // delay(5000);
}

// #include <AccelStepper.h>

// // Cấu hình chân Step và Dir cho trục Y
// #define PIN_Y_STEP 4
// #define PIN_Y_DIR 2

// // Khởi tạo đối tượng stepper kiểu DRIVER (step/dir)
// AccelStepper stepperY(AccelStepper::DRIVER, PIN_Y_STEP, PIN_Y_DIR);

// // Thông số chuyển động
// const int maxSpeedY = 1000;       // tốc độ tối đa (steps/sec)
// const int accelY = 500;           // gia tốc (steps/sec^2)
// const long travelDistance = 2000; // số bước cho một chu kỳ đi hoặc về

// bool goingForward = true; // trạng thái di chuyển

// void setup()
// {
//     Serial.begin(9600);
//     stepperY.setMaxSpeed(maxSpeedY);
//     stepperY.setAcceleration(accelY);

//     // Thiết lập vị trí hiện tại là 0
//     stepperY.setCurrentPosition(0);
//     stepperY.moveTo(travelDistance);

//     Serial.println("Stepper Y bắt đầu chạy tới lui...");
// }

// void loop()
// {
//     // Gọi run để stepper thực hiện di chuyển
//     stepperY.run();

//     // Khi đạt đến vị trí đích, đổi hướng
//     if (stepperY.distanceToGo() == 0)
//     {
//         if (goingForward)
//         {
//             stepperY.moveTo(0); // quay về gốc
//             goingForward = false;
//             Serial.println("Quay về gốc...");
//         }
//         else
//         {
//             stepperY.moveTo(travelDistance); // đi tới lại
//             goingForward = true;
//             Serial.println("Đi tới vị trí xa...");
//         }
//     }
// }
// #include <Arduino.h>

// #define SERVO_PIN 25

// void setup()
// {
//     Serial.begin(115200);
//     ledcSetup(0, 50, 16);        // Channel 0, 50Hz, 16-bit resolution
//     ledcAttachPin(SERVO_PIN, 0); // Attach pin 25 to channel 0
//     Serial.println("[SET UP]: Raw PWM Servo Setup Done!");
// }

// void setServoAngle(int angle)
// {
//     // Map angle (0-180) to pulse width (500-2500us)
//     int pulseWidth = map(angle, 0, 180, 500, 2500);
//     // Convert pulse width to duty cycle (for 16-bit, 50Hz)
//     uint32_t duty = (pulseWidth * 65535) / 20000;
//     ledcWrite(0, duty);
// }

// void loop()
// {
//     Serial.println("[PROCESS]: Lifting Pen");
//     setServoAngle(90); // Lift pen
//     delay(1000);

//     Serial.println("[PROCESS]: Dropping Pen");
//     setServoAngle(0); // Drop pen
//     delay(1000);
// }
