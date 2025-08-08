#include "AppController.h"

void taskRunSD(void *pvParameters)
{
    Serial.println("[TASK]: G-code processing task started on Core 1");

    for (;;)
    {
        // Suspend when SD status screen is not active; resume will continue here
        if (AppController::getInstance().currentState != State::SD_STATUS)
        {
            Serial.println("[TASK]: G-code processing task suspended");
            vTaskSuspend(NULL);
            Serial.println("[TASK]: G-code processing task resumed");
            continue;
        }

        // Process one step of G-code
        AutoModeController::getInstance().runSD(AppController::getInstance().workingFlag);

        // Small delay to yield CPU
        vTaskDelay(pdMS_TO_TICKS(5));
    }

    // Should never reach here
}
