/*******************************************************************************
 * @file    ButtonHAL.cpp
 * @brief   Định nghĩa các hàm trong file header
 * @version 1.0
 * @date    2025-06-19
 * @author  Do Duc Nghia
 ******************************************************************************/
/*================================================ [ INCLUDE LIBRARY ] ==================================================*/
#include "ButtonHAL.h"
#include "Pins.h"

/*================================================= [ DEFINITION ] ==================================================*/

ButtonHAL::ButtonHAL()
{
}

/*<===================================================>*/

ButtonHAL &ButtonHAL::getInstance()
{
    static ButtonHAL instance;
    return instance;
}

/*<===================================================>*/

void ButtonHAL::setup()
{
    // Cấu hình chân GPIO làm output

    // Setup
    pinMode(PIN_MENU_BUTTON, INPUT);

    // Logging
    Serial.println("[SET UP]: Button Done!");
}

/*<===================================================>*/

bool ButtonHAL::isButtonPressed()
{
    if (digitalRead(PIN_MENU_BUTTON) == HIGH)
    {
        return true;
    }
    return false;
}

/*<===================================================>*/

bool ButtonHAL::isButtonHeld3s(bool (*isButtonPressed)())
{
    // Logging
    Serial.println("Check nếu nút được giữ 3 giây!");

    const unsigned long holdDuration = 3000; // 3 giây
    unsigned long startTime = millis();

    // Kiểm tra nếu nút đang được nhấn
    if (isButtonPressed() == true)
    {
        // Chờ cho đến khi hết thời gian hold hoặc bị nhả ra
        while (millis() - startTime < holdDuration)
        {
            if (isButtonPressed() == false)
            {
                // Bị nhả ra trước khi đủ 2 giây
                return false;
            }
        }
        // Nếu qua được vòng while nghĩa là nút đã giữ đủ 2 giây
        return true;
    }

    // Không nhấn ngay từ đầu
    return false;
}

/*<===================================================>*/
