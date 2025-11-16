#pragma once

#include <Arduino.h>

// Stepper Motor - Trục X
#define PIN_X_STEP 27
#define PIN_X_DIR 14

// Stepper Motor - Trục Y
#define PIN_Y_STEP 4
#define PIN_Y_DIR 2

// Servo nhấc/hạ bút
#define PIN_SERVO 25

// Buzzer
#define PIN_BUZZER 13

// LCD (I2C)
#define LCD_SDA_PIN 21
#define LCD_SCL_PIN 22

// PS4 Controller - Bluetooth, không cần pin cụ thể

// Rotary Encoder (nếu dùng)
#define PIN_CLK 32
#define PIN_DT 33
#define PIN_SW 26

// Nút chọn menu riêng (tuỳ chọn)
#define PIN_MENU_BUTTON 15

// SD Card (SPI)
#define PIN_SD_CS 5

// RTC (I2C dùng chung SDA/SCL)
#define RTC_SDA_PIN 21
#define RTC_SCL_PIN 22

// Công tắc hành trình
#define PIN_ENDSTOP_X 34
#define PIN_ENDSTOP_Y 35
