#ifndef GCODEPARSER_H
#define GCODEPARSER_H

#include <Arduino.h>
#include "MotionController.h"

class GCodeParser{
public:
    // Constructor
    GCodeParser(MotionController &mc);
    void parseLine(const String &line);  // Phan tich  1 dong Gcode
private:
    MotionController &_mc;
    void handleGcode(int code, const String &args);
};

#endif

