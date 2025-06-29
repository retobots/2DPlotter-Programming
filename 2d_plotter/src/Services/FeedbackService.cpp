#include <Arduino.h>
#include "HAL/LcdHAL.h"
#include "Services/FeedbackService.h
#define BUZZER 4 // chan buzzer

FeedbackService::FeedbackService() {}

FeedbackService& FeedbackService::getInstance() {
  static FeedbackService instance;
  return instance;
}

void FeedbackService::setup(){
  LcdHAL::getInstance().setup();
  const int buzzerPin=BUZZER;
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
}

void FeedbackService::showStatus(const char* message){
  LcdHAL::getInstance().print(std::string(message));
}

void FeedbackService::beep(uint8_t type){
  const int buzzerPin=BUZZER;
  switch(type){
    case 1:
      tone(buzzerPin, 1000, 200);
      break;
    case 2: //
      tone(buzzerPin, 1000, 200);
      delay(300);
      tone(buzzerPin, 1000, 200);
      break;
    case 3: //Eror
      for (int i = 0; i < 3; i++) {
        tone(buzzerPin, 800, 500);
        delay(600);
      }
      break;
    default:
      break;
  }
}