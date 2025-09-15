/*======================================================== [INCLUDE LIBRARY] ===================================================*/
#include "StepperHAL.hpp"

/*=====================================================  [FUNCTION DEFINITIONS] ================================================*/

/*========================================================== ( Hàm Setup ) =====================================================*/

void setupStepper()
{

    // Setup Chân Điều Khiển X
    AccelStepper stepperX(AccelStepper::DRIVER, X_STEP, X_DIR);

    // Setup chân điều khiển Y
    AccelStepper stepperY(AccelStepper::DRIVER, Y_STEP, Y_DIR);

    // Set Thông Số
    pinMode(ENABLE, OUTPUT);
    digitalWrite(ENABLE, LOW);

    stepperX.setMaxSpeed(MAX_SPEED);        // Tốc độ tối đa (step/giây)
    stepperX.setAcceleration(ACCELERATION); // Gia tốc (step/giây^2)
}

/*======================================================== ( Hàm Dẫn Hướng) ====================================================*/

void moveXYTo(float x, float y)
{
    stepperX.moveTo(x);
    stepperY.moveTo(y);
}

/*====================================================== ( Hàm Chạy Động Cơ ) ==================================================*/

void runXY()
{
    stepperX.run();
    stepperY.run();
}

/*======================================================= ( Hàm Điều Khiển) ====================================================*/

void runXYBlocking()
{
    while (stepperX.isRunning() || stepperY.isRunning())
    {
        runXY();
    }
}