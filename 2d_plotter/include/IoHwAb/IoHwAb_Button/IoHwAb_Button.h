/*************************************************************************************************************************
 * @file    IoHwAb_Button.h
 * @brief   Declarations of functions in the header file
 * @version 1.0
 * @date    2025-06-19
 * @author  Do Duc Nghia
 ************************************************************************************************************************/
#pragma once

#include <Arduino.h>

/*************************************************************************************************************************
 * @class   IoHwAb_Button
 * @brief   Class containing functions to control button input
 * @details This structure contains button object parameters and functions to read button state and setup parameters
 ************************************************************************************************************************/
class IoHwAb_Button
{
private:
    // Constructor
    IoHwAb_Button();

public:
    static IoHwAb_Button &getInstance();
    void setup();
    bool isButtonPressed();
    bool isButtonHeld3s(bool (*isButtonPressed)());
};