#include "BuzzerHAL.h"

BuzzerHAL::BuzzerHAL() {}

BuzzerHAL& BuzzerHAL::getInstance(){
    static BuzzerHAL instance;
    return instance;
}

void BuzzerHAL::setup(){
    pinMode(buzzerPin, OUTPUT);
    digitalWrite(buzzerPin, LOW);
}

void BuzzerHAL::beepStart(){
    tone(buzzerPin, 1000, 200); // 1 beep ngan 200ms
}

void BuzzerHAL::beepFinish(){
    tone(buzzerPin, 1000, 200);
    delay(300);
    tone(buzzerPin, 1000, 200); // 2 beep ngan
}

void BuzzerHAL::beepError(){
    for (int i = 0; i < 3; i++){
        tone(buzzerPin, 800, 500); // 3 beep dai
        delay(600);
    }
}