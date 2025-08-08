#include <Arduino.h>
#include "AppController.h"

void taskUI(void *pvParameters)
{
    Serial.println("[TASK]: UI task started on Core 0");

    unsigned long lastUIUpdate = 0;
    const unsigned long UI_UPDATE_INTERVAL = 200; // Update UI every 200ms

    for (;;)
    {
        // If SD status screen is not active, suspend until resumed
        if (AppController::getInstance().currentState != State::SD_STATUS)
        {
            Serial.println("[TASK]: UI task suspended");
            vTaskSuspend(NULL);
            // When resumed, reset timers/state
            Serial.println("[TASK]: UI task resumed");
            lastUIUpdate = millis();
            continue;
        }

        // Read encoder frequently for responsive input (no delay here)
        IoHwAb_Encoder::getInstance().readEncoder();

        // Handle encoder input immediately
        if (IoHwAb_Encoder::getInstance().scrollUp() || IoHwAb_Encoder::getInstance().scrollDown())
        {
            IoHwAb_Buzzer::getInstance().beepOnce();
            bool isUp = IoHwAb_Encoder::getInstance().scrollUp();
            AppController::getInstance().workingFlag = (isUp) ? (AppController::getInstance().workingFlag + 1) % 3 : (AppController::getInstance().workingFlag + 2) % 3;
            IoHwAb_LCD::getInstance().clear();

            // Immediate UI update
            UIMenuService::getInstance().statusScreen(
                AppController::getInstance().selectedFile,
                AutoModeController::getInstance().getPercentage(),
                AutoModeController::getInstance().getTime(),
                AppController::getInstance().workingFlag,
                AppController::getInstance().workingStateFlag);

            lastUIUpdate = millis(); // Reset UI update timer
        }

        if (IoHwAb_Encoder::getInstance().isRotaryPressed())
        {
            IoHwAb_Buzzer::getInstance().beepOnce();

            if (AppController::getInstance().workingFlag == WORKING_STATE)
            {
                // Toggle Pause/Resume without leaving this screen
                if (AppController::getInstance().workingStateFlag == PAUSE)
                {
                    AutoModeController::getInstance().pauseSD();
                    AppController::getInstance().workingStateFlag = CONTINUE; // Label now shows RESUME
                }
                else
                {
                    AutoModeController::getInstance().continueSD();
                    AppController::getInstance().workingStateFlag = PAUSE; // Label now shows PAUSE
                }
            }
            else if (AppController::getInstance().workingFlag == CANCEL)
            {
                // Cancel: stop processing and go back to SD menu; next run starts from the beginning
                Serial.println("[UI]: Cancel button pressed");
                AutoModeController::getInstance().cancelSD();
                AutoModeController::getInstance().resetSD();
                AppController::getInstance().currentState = State::SD_MENU;
                Serial.println("[UI]: State changed to SD_MENU");
                // Next iteration will suspend because state != SD_STATUS
                continue;
            }
            else
            {
                Serial.println("[UI]: Back button pressed");
                AppController::getInstance().currentState = State::SD_MENU;
                Serial.println("[UI]: State changed to SD_MENU");
                // Next iteration will suspend because state != SD_STATUS
                continue;
            }

            // Force immediate UI update when user interacts
            UIMenuService::getInstance().statusScreen(
                AppController::getInstance().selectedFile,
                AutoModeController::getInstance().getPercentage(),
                AutoModeController::getInstance().getTime(),
                AppController::getInstance().workingFlag,
                AppController::getInstance().workingStateFlag);

            lastUIUpdate = millis(); // Reset UI update timer
        }

        // Update UI periodically (not every loop iteration)
        if (millis() - lastUIUpdate >= UI_UPDATE_INTERVAL)
        {
            UIMenuService::getInstance().statusScreen(
                AppController::getInstance().selectedFile,
                AutoModeController::getInstance().getPercentage(),
                AutoModeController::getInstance().getTime(),
                AppController::getInstance().workingFlag,
                AppController::getInstance().workingStateFlag);

            lastUIUpdate = millis();
        }

        // Very short delay to prevent task from consuming too much CPU
        vTaskDelay(pdMS_TO_TICKS(5));
    }

    // Never return from a task (can crash). Suspend and let controller resume when needed.
    Serial.println("[TASK]: UI task ended -> suspending");
    vTaskSuspend(NULL);
    // Should never reach here
    for (;;)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}