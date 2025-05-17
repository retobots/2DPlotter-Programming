/*======================================================== [INCLUDE LIBRARY] ===================================================*/
#include "StepperHAL.hpp"

/*=====================================================  [FUNCTION DEFINITIONS] ================================================*/

/*========================================================== ( Hàm Setup ) =====================================================*/

void setupStepper()
{
    // Setup Chân Điều Khiển X
    pinMode(X_DIR, OUTPUT);
    pinMode(X_STEP, OUTPUT);

    // Setup chân điều khiển Z
    pinMode(Y_DIR, OUTPUT);
    pinMode(Y_STEP, OUTPUT);

    // Setup khởi động
    pinMode(ENABLE, OUTPUT);
    digitalWrite(ENABLE, LOW);
}

/*===================================================== ( Hàm Điều Khiển Trục X ) ==============================================*/

void X_Draw()
{
    digitalWrite(X_STEP, HIGH);
    delay(STEP_DELAY);
    digitalWrite(X_STEP, LOW);
    delay(STEP_DELAY);
}

/*===================================================== ( Hàm Điều Khiển Trục Y ) ==============================================*/

void Y_Draw()
{
    digitalWrite(Y_STEP, HIGH);
    delay(STEP_DELAY));
    digitalWrite(Y_STEP, LOW);
    delay(STEP_DELAY);
}
