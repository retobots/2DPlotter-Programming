#include "MotionController.h"

MotionController::MotionController(int stepsPerRevX, int stepsPerRevY, int motorPinX1, int motorPinX2, int motorPinY1, int motorPinY2, int servoPin): 
    stepperX(stepsPerRevX, motorPinX1, motorPinX2), stepperY(stepsPerRevY, motorPinY1, motorPinY2), currentX(0), currentY(0), feedRate(1000)
{
    penServo.attach(servoPin);
    servoUpPos = 90;     
    servoDownPos = 0;
}

void MotionController::begin(){stepperX.setSpeed(60); stepperY.setSpeed(60); penUp();}   // Cai dat toc do cho dc step (r/m) va trang thai servo ban dau

void MotionController::linearMove(float x, float y, float feedRate){
    // Lay toa do cu suy ra toa do moi de di chuyen
    float dx=x-currentX;
    float dy=y-currentY;
    long stepsX=abs(dx)*100; // chuyen step sang buoc(100step/buoc)
    long stepsY=abs(dy)*100;
    // CHUA HOAN THIEN. ham sau de dieu chinh toc do sao cho 2 dong co di chuyen cung luc va dong thoi
    if(stepsX>stepsY)
        stepperY.setSpeed(stepperY.speed()*(float)stepsY/stepsX);
    else
        stepperX.setSpeed(stepperX.speed()*(float)stepsX/stepsY);

    // Phuong phap Bresenham (Docs: https://rosettacode.org/wiki/Bitmap/Bresenham's_line_algorithm#C)
    long n=max(stepsX,stepsY);
    for (long i=0; i<n; i++) {
        if (i<stepsX) stepperX.step((dx>0)?1:-1);
        if (i<stepsY) stepperY.step((dy>0)?1:-1);
    }
    currentX = x;
    currentY = y;
    stepperX.setSpeed(60);
    stepperY.setSpeed(60);
}

void MotionController::circularMove(float targetX, float targetY, float centerI, float centerJ, bool clockwise, float feedRate){
    // Xac dinh vi tri hien tai de xem nhu la diem MOC de ve duong tron theo lenhj Gcode
    float centerX=currentX+centerI;
    float centerY=currentY+centerJ;
    // Pytago =)) --> R
    float radius=sqrt(centerI*centerI+centerJ*centerJ);
    // Tinh goc bat dau va ket thuc'
    float startAngle=atan2(currentY-centerY, currentX-centerX);
    float endAngle=atan2(targetY-centerY, targetX-centerX);
    float sweep=(clockwise?-1:1)*(endAngle-startAngle);
    // Neu theo chieu kim dong ho => sweep am, nguoc chieu kim dong ho => sweep duong
    if(clockwise&&sweep>0) sweep-=2*PI;
    if(!clockwise&&sweep<0) sweep+=2*PI;
    int segments=50; // so duong thang cau thanh cung tron( co the tang de duong tron dep hon!)
    for(int i=1; i<=segments; i++){
        float ang=startAngle+sweep*i/segments;
        float xi=centerX+radius*cos(ang);
        float yi=centerY+radius*sin(ang);
        long stepX=abs((xi-currentX)*100);
        long stepY=abs((yi-currentY)*100);
        if (stepX) stepperX.step((xi>currentX)?1:-1);
        if (stepY) stepperY.step((yi>currentY)?1:-1);
        currentX=xi;currentY=yi;
    }
}

void MotionController::penUp() 
{ penServo.write(servoUpPos); }

void MotionController::penDown() 
{ penServo.write(servoDownPos); }

void MotionController::goHome() 
{ linearMove(0, 0, feedRate); }

void MotionController::pauseMotion() 
{ delay(1000); }//DANG SUY NGHI BO SUNG 
