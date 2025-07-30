#pragma

#include <Arduino.h>

#define ENABLE 7

// Stepper Motor - Trục X
#define PIN_X_STEP 10
#define PIN_X_DIR 11

// Stepper Motor - Trục Y
#define PIN_Y_STEP 20
#define PIN_Y_DIR 19

// Servo nhấc/hạ bút
#define PIN_SERVO 25

// Buzzer
#define PIN_BUZZER 13

// LCD (I2C)
#define LCD_SDA_PIN 21
#define LCD_SCL_PIN 22

// PS4 Controller - Bluetooth, không cần pin cụ thể

// Rotary Encoder (nếu dùng)
#define PIN_CLK 7
#define PIN_DT 6
#define PIN_SW 9

// Nút chọn menu riêng (tuỳ chọn)
#define PIN_MENU_BUTTON

// SD Card (SPI)
#define PIN_SD_CS 23

// RTC (I2C dùng chung SDA/SCL)
#define RTC_SDA_PIN 21
#define RTC_SCL_PIN 22
