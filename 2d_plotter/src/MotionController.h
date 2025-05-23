#ifndef MOTIONCONTROLLER_H
#define MOTIONCONTROLLER_H
#include <Arduino.h>
#include <Stepper.h>
#include <Servo.h>

class MotionController{
public:
    MotionController(int stepsPerRevX, int stepsPerRevY, int motorPinX1, int motorPinX2, int motorPinY1, int motorPinY2, int servoPin);
    void begin();
    void linearMove(float x, float y, float feedRate); // Tuyen Tinh
    void circularMove(float targetX, float targetY, float centerI, float centerJ, bool clockwise, float feedRate);
    void penUp();   // Nang but
    void penDown(); // Ha but
    void goHome();  // Ve home
    void pauseMotion(); // STOP
    float getCurrentX() const {return currentX;}
    float getCurrentY() const {return currentY;}
    float getFeedRate() const {return feedRate;}
    void setFeedRate(float f) {feedRate = f;}
private:
    Stepper stepperX;
    Stepper stepperY;
    Servo penServo;
    float currentX, currentY;
    float feedRate;
    int servoUpPos, servoDownPos;
    void stepMove(Stepper &motor, long steps);
};
#endif
