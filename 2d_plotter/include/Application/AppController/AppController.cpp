#include "AppController.h"

AppController::AppController()
{
  // Constructor
}

AppController &AppController::getInstance()
{
  static AppController instance;
  return instance;
}

void AppController::setup()
{
  UIMenuService::getInstance().setup();
  AutoModeController::getInstance().setup();
  ManualModeController::getInstance().setup();

  Serial.println("[SET UP]: AppController Done!");

  Serial.println("[OPENING]: Welcome Screen");
  opening();
}

/*<===================================================>*/

void AppController::runSDMenu()
{
  uint8_t ar_idx = 0;
  int startIndex = 0;
  int state = State::SD_MENU;
  fileList = IoHwAb_SD::getInstance().loadFileListFromSD();

  IoHwAb_LCD::getInstance().clear();
  Serial.printf("currentState: %d\n", state);
  UIMenuService::getInstance().sdModeScreen(fileList, 0, ar_idx, startIndex, fileList.size(), state, selectedFile);

  while (1)
  {
    if (state == State::AUTO_MODE)
    {
      currentState = State::AUTO_MODE;
      break;
    }

    if (state == State::SD_STATUS)
    {
      currentState = State::SD_STATUS;
      break; // Remove recursive call, let main loop handle it
    }

    IoHwAb_Encoder::getInstance().readEncoder();

    if (IoHwAb_Encoder::getInstance().scrollUp())
    {
      IoHwAb_Buzzer::getInstance().beepOnce();
      IoHwAb_LCD::getInstance().clear();
      UIMenuService::getInstance().sdModeScreen(fileList, -1, ar_idx, startIndex, fileList.size(), state, selectedFile);
    }
    else if (IoHwAb_Encoder::getInstance().scrollDown())
    {
      IoHwAb_Buzzer::getInstance().beepOnce();
      IoHwAb_LCD::getInstance().clear();
      UIMenuService::getInstance().sdModeScreen(fileList, 1, ar_idx, startIndex, fileList.size(), state, selectedFile);
    }
    else if (IoHwAb_Encoder::getInstance().isRotaryPressed())
    {
      IoHwAb_Buzzer::getInstance().beepOnce();
      UIMenuService::getInstance().sdModeScreen(fileList, 2, ar_idx, startIndex, fileList.size(), state, selectedFile);
    }

    // Add small delay to prevent tight loop
    vTaskDelay(pdMS_TO_TICKS(5)); // Reduced for better encoder responsiveness
  }
}

/*<===================================================>*/

void AppController::runMainMenu()
{
  IoHwAb_LCD::getInstance().clear();
  UIMenuService::getInstance().modeScreen(modeFlag);

  while (1)
  {
    IoHwAb_Encoder::getInstance().readEncoder();

    if (IoHwAb_Encoder::getInstance().scrollUp() || IoHwAb_Encoder::getInstance().scrollDown())
    {
      IoHwAb_Buzzer::getInstance().beepOnce();
      modeFlag ^= 1; // Toggle nhanh
      IoHwAb_LCD::getInstance().clear();
      UIMenuService::getInstance().modeScreen(modeFlag);
    }

    if (IoHwAb_Encoder::getInstance().isRotaryPressed())
    {
      IoHwAb_Buzzer::getInstance().beepOnce();
      currentState = (modeFlag == AUTOMODE) ? State::AUTO_MODE : State::MANUAL_MODE;
      break;
    }

    // Add small delay to prevent tight loop
    vTaskDelay(pdMS_TO_TICKS(5)); // Reduced for better encoder responsiveness
  }
}

/*<===================================================>*/

void AppController::runAutoMode()
{
  IoHwAb_LCD::getInstance().clear();
  UIMenuService::getInstance().automodeScreen(autoModeFlag);

  while (1)
  {
    IoHwAb_Encoder::getInstance().readEncoder();

    if (IoHwAb_Encoder::getInstance().scrollUp() || IoHwAb_Encoder::getInstance().scrollDown())
    {
      IoHwAb_Buzzer::getInstance().beepOnce();
      bool isUp = IoHwAb_Encoder::getInstance().scrollUp();
      autoModeFlag = (isUp) ? (autoModeFlag + 2) % 3 : (autoModeFlag + 1) % 3;
      IoHwAb_LCD::getInstance().clear();
      UIMenuService::getInstance().automodeScreen(autoModeFlag);
    }

    if (IoHwAb_Encoder::getInstance().isRotaryPressed())
    {
      IoHwAb_Buzzer::getInstance().beepOnce();
      switch (autoModeFlag)
      {
      case UGS:
        currentState = State::UGS_MENU;
        break;
      case SD:
        currentState = State::SD_MENU;
        break;
      default:
        currentState = State::MAIN_MENU;
      }
      break;
    }

    // Add small delay to prevent tight loop
    vTaskDelay(pdMS_TO_TICKS(5)); // Reduced for better encoder responsiveness
  }
}

/*<===================================================>*/

void AppController::runManualMode()
{
  IoHwAb_LCD::getInstance().clear();
  IoHwAb_PS4::getInstance().reconnect();
  while (1)
  {

    IoHwAb_Encoder::getInstance().readEncoder();

    if (IoHwAb_PS4::getInstance().isConnected() && !isPS4Initialized)
    {
      IoHwAb_LCD::getInstance().clear();
      UIMenuService::getInstance().PS4ModeScreen(1);
      isPS4Initialized = true;
    }

    ManualModeController::getInstance().run();

    if (IoHwAb_Encoder::getInstance().isRotaryPressed())
    {
      IoHwAb_Buzzer::getInstance().beepOnce();
      currentState = State::MAIN_MENU;
      Serial.println("[UI]: Back to Main Menu");
      isPS4Initialized = false;
      break;
    }

    // Keep loop responsive for AccelStepper::runSpeed() tick rate
    // 50 ms limited speed severely (~20 Hz). 1 ms allows up to ~1 kHz step rate per axis.
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}

/*<===================================================>*/

void AppController::runUGSMenu()
{
  IoHwAb_LCD::getInstance().clear();
  UIMenuService::getInstance().UGSModeScreen();

  while (1)
  {
    IoHwAb_Encoder::getInstance().readEncoder();
    AutoModeController::getInstance().runSerial();

    if (IoHwAb_Encoder::getInstance().isRotaryPressed())
    {
      IoHwAb_Buzzer::getInstance().beepOnce();

      currentState = State::AUTO_MODE;
      delay(200);
      break;
    }

    // Add small delay to prevent tight loop
    vTaskDelay(pdMS_TO_TICKS(5)); // Reduced for better encoder responsiveness
  }
}

/*<===================================================>*/

void AppController::run()
{
  Serial.println("[PROCESS]: Running Application");

  switch (currentState)
  {
  case MAIN_MENU:
    runMainMenu();
    break;
  case AUTO_MODE:
    runAutoMode();
    break;
  case MANUAL_MODE:
    runManualMode();
    break;
  case UGS_MENU:
    runUGSMenu();
    break;
  case SD_MENU:
    runSDMenu();
    break;
  case SD_STATUS:
    runSDMode();
    break;
  }

  // Add delay to prevent tight loop and reduce CPU usage
  vTaskDelay(pdMS_TO_TICKS(5)); // Reduced for better overall responsiveness
}

/*<===================================================>*/

void AppController::opening()
{
  IoHwAb_Buzzer::getInstance().startingSoundBuzzer();
  UIMenuService::getInstance().welcomeScreen();
  delay(2000);

  IoHwAb_LCD::getInstance().clear();
  UIMenuService::getInstance().modeScreen(modeFlag);

  Serial.println("[OPENING]: Opening Done!");
}

/*<===================================================>*/

void AppController::runSDMode()
{
  Serial.println("[PROCESS]: Entering SD Mode");

  // Ensure fresh control state for a new run
  workingStateFlag = PAUSE;    // First action means "PAUSE" (press to pause)
  workingFlag = WORKING_STATE; // Highlight the first option by default

  // 1. Clear màn hình, load file
  IoHwAb_LCD::getInstance().clear();
  AutoModeController::getInstance().getGcodeFile("/" + AppController::getInstance().selectedFile);

  Serial.println("[PROCESS]: File loaded and G-code parsed");

  // 2. Hiển thị màn hình ban đầu
  UIMenuService::getInstance().statusScreen(
      AppController::getInstance().selectedFile,
      0,
      "00:00:00",
      AppController::getInstance().workingFlag,
      AppController::getInstance().workingStateFlag);

  // Check available memory before creating tasks
  Serial.printf("[DEBUG]: Free heap before task creation: %d bytes\n", ESP.getFreeHeap());
  Serial.printf("[DEBUG]: Free stack space: %d bytes\n", uxTaskGetStackHighWaterMark(NULL));

  // 3. Only create tasks if they don't already exist, otherwise resume them
  if (taskRunSDHandle == NULL)
  {
    BaseType_t result1 = xTaskCreatePinnedToCore(taskRunSD, "RunSD", 4096, NULL, 2, &taskRunSDHandle, 1); // Core 1
    if (result1 != pdPASS)
    {
      Serial.printf("[ERROR]: Failed to create RunSD task! Result: %d\n", result1);
      Serial.printf("[ERROR]: Free heap after failure: %d bytes\n", ESP.getFreeHeap());
      currentState = State::SD_MENU;
      return;
    }
    else
    {
      Serial.println("[DEBUG]: RunSD task created");
    }
  }
  else
  {
    // Resume if previously suspended
    vTaskResume(taskRunSDHandle);
    Serial.println("[DEBUG]: RunSD task resumed");
  }

  if (taskUIHandle == NULL)
  {
    BaseType_t result2 = xTaskCreatePinnedToCore(taskUI, "UI", 4096, NULL, 2, &taskUIHandle, 0); // Core 0
    if (result2 != pdPASS)
    {
      Serial.printf("[ERROR]: Failed to create UI task! Result: %d\n", result2);
      Serial.printf("[ERROR]: Free heap after failure: %d bytes\n", ESP.getFreeHeap());
      // If UI task failed but RunSD exists, suspend it to keep system stable
      if (taskRunSDHandle != NULL)
      {
        vTaskSuspend(taskRunSDHandle);
      }
      currentState = State::SD_MENU;
      return;
    }
    else
    {
      Serial.println("[DEBUG]: UI task created");
    }
  }
  else
  {
    vTaskResume(taskUIHandle);
    Serial.println("[DEBUG]: UI task resumed");
  }

  Serial.println("[DEBUG]: Tasks ready (created or resumed)");

  // 5. Đợi đến khi state đổi (Cancel/Back) → thoát về menu
  while (AppController::getInstance().currentState == State::SD_STATUS)
  {
    vTaskDelay(pdMS_TO_TICKS(100)); // Poll mỗi 100ms
  }

  Serial.println("[DEBUG]: Exiting SD mode - tasks will suspend themselves for reuse");
  Serial.printf("[DEBUG]: Free heap after SD mode: %d bytes\n", ESP.getFreeHeap());
}
