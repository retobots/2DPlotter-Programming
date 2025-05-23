#include "SDReader.h"
#include "GCodeParser.h"
#include "MotionController.h"
#include <Arduino.h>

#define SD_CS_PIN 10  // Chan CS cua module doc SD card
#define STEPS_REV_X 200  // Ví dụ: 200 step/vong cho stepper X
#define STEPS_REV_Y 200  // y nhu tren nhma voi Y
#define MOTOR_X1 2
#define MOTOR_X2 3
#define MOTOR_Y1 4
#define MOTOR_Y2 5
#define SERVO_PIN 6

SDReader sd(SD_CS_PIN);
MotionController motion(STEPS_REV_X, STEPS_REV_Y, MOTOR_X1, MOTOR_X2, MOTOR_Y1, MOTOR_Y2, SERVO_PIN);
GCodeParser parser(motion);

void setup(){
    Serial.begin(115200);
    if(!sd.begin()){
        Serial.println("EKhoi tao Sd that bai");
        while(1);
    }
    motion.begin();
    File gcode=sd.openFirstTxtFile();
    if(!gcode){
        Serial.println("ko co file gcode");
        while(1);
    }
    while(gcode.available()){
        String line = sd.readLine(gcode);
        Serial.print("GCode: "); Serial.println(line);
        parser.parseLine(line);
    }
    gcode.close();
}
void loop(){
}
