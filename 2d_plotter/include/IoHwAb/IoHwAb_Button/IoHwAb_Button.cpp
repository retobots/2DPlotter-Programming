/*************************************************************************************************************************
 * @file    IoHwAb_Button.cpp
 * @brief   Definitions of functions in the header file
 * @version 1.0
 * @date    2025-06-19
 * @author  Do Duc Nghia
 ************************************************************************************************************************/
#include "IoHwAb_Button.h"
#include "Pins.h"

/*************************************************************************************************************************
 * @brief Constructor
 ************************************************************************************************************************/
IoHwAb_Button::IoHwAb_Button()
{
}

/*************************************************************************************************************************
 * @brief Get the singleton instance
 ************************************************************************************************************************/
IoHwAb_Button &IoHwAb_Button::getInstance()
{
    static IoHwAb_Button instance;
    return instance;
}

/*************************************************************************************************************************
 * @brief Setup the button
 ************************************************************************************************************************/
void IoHwAb_Button::setup()
{
    Serial.println("[SET UP]: Button Starting Setup!");

    // Setup
    pinMode(PIN_MENU_BUTTON, INPUT);

    // Logging
    Serial.println("[SET UP]: Button Done!");
}

/*************************************************************************************************************************
 * @brief Check if the button is pressed
 ************************************************************************************************************************/

bool IoHwAb_Button::isButtonPressed()
{
    if (digitalRead(PIN_MENU_BUTTON) == HIGH)
    {
        return true;
    }
    return false;
}

/*************************************************************************************************************************
 * @brief Check if the button is held for 3 seconds
 *
 * @param isButtonPressed Pointer to the function that checks if the button is pressed
 * @return true if the button is held for 3 seconds, false otherwise
 ************************************************************************************************************************/
bool IoHwAb_Button::isButtonHeld3s(bool (*isButtonPressed)())
{
    const unsigned long holdDuration = 3000; // 3 seconds
    unsigned long startTime = millis();

    // Check if the button is currently pressed
    if (isButtonPressed() == true)
    {
        // Wait until the hold duration is reached or the button is released
        while (millis() - startTime < holdDuration)
        {
            if (isButtonPressed() == false)
            {
                // Released before 3 seconds
                return false;
            }
        }
        // If the while loop is passed, the button has been held for 3 seconds
        return true;
    }

    // Not pressed from the beginning
    return false;
}