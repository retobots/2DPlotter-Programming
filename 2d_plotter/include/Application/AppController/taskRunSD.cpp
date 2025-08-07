#include "AppController.h"

void taskRunSD(void *pvParameters)
{
    while (1)
    {
        AutoModeController::getInstance().runSD(AppController::getInstance().workingFlag);
        vTaskDelay(pdMS_TO_TICKS(10)); // Adjust delay as needed
    }
}
