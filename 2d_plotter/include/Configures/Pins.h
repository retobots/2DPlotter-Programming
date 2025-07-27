#pragma

#include <Arduino.h>

#define ENABLE 8

// Stepper Motor - Trục X
#define PIN_X_STEP 2
#define PIN_X_DIR 3

// Stepper Motor - Trục Y
#define PIN_Y_STEP 4
#define PIN_Y_DIR 5

// Servo nhấc/hạ bút
#define PIN_SERVO 6

// Buzzer
#define PIN_BUZZER 27

// LCD (I2C)
#define LCD_SDA_PIN 21
#define LCD_SCL_PIN 22

// PS4 Controller - Bluetooth, không cần pin cụ thể

// Rotary Encoder (nếu dùng)
#define PIN_CLK 18
#define PIN_DT 19
#define PIN_SW 5

// Nút chọn menu riêng (tuỳ chọn)
#define PIN_MENU_BUTTON 19

// SD Card (SPI)
#define PIN_SD_CS 13

// RTC (I2C dùng chung SDA/SCL)
#define RTC_SDA_PIN 21
#define RTC_SCL_PIN 22
