#include <Arduino.h>
#include "AppController.h"

void taskUI(void *pvParameters)
{
    IoHwAb_Encoder::getInstance().readEncoder();

    UIMenuService::getInstance().statusScreen(
        AppController::getInstance().selectedFile,
        AutoModeController::getInstance().getPercentage(),
        AutoModeController::getInstance().getTime(),
        AppController::getInstance().workingFlag);

    if (IoHwAb_Encoder::getInstance().scrollUp() || IoHwAb_Encoder::getInstance().scrollDown())
    {
        IoHwAb_Buzzer::getInstance().beepOnce();
        bool isUp = IoHwAb_Encoder::getInstance().scrollUp();
        AppController::getInstance().workingFlag = (isUp) ? (AppController::getInstance().workingFlag + 1) % 3 : (AppController::getInstance().workingFlag + 2) % 3;
        IoHwAb_LCD::getInstance().clear();

        UIMenuService::getInstance().statusScreen(
            AppController::getInstance().selectedFile,
            AutoModeController::getInstance().getPercentage(),
            AutoModeController::getInstance().getTime(),
            AppController::getInstance().workingFlag);
    }

    if (IoHwAb_Encoder::getInstance().isRotaryPressed())
    {
        IoHwAb_Buzzer::getInstance().beepOnce();

        if (AppController::getInstance().workingFlag == WORKING_STATE)
        {
            if (AppController::getInstance().workingStateFlag == PAUSE)
            {
                AutoModeController::getInstance().pauseSD();
                AppController::getInstance().workingStateFlag = CONTINUE;
            }
            else
            {
                AutoModeController::getInstance().continueSD();
                AppController::getInstance().workingStateFlag = PAUSE;
            }
        }
        else if (AppController::getInstance().workingFlag == CANCEL)
        {
            AutoModeController::getInstance().cancelSD();
            AutoModeController::getInstance().resetSD();
        }
        else
        {
            AppController::getInstance().currentState = State::SD_MENU;
            vTaskDelete(NULL); // Thoát task UI
        }
    }

    vTaskDelay(pdMS_TO_TICKS(50));
}
