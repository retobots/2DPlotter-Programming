#ifndef GCODE_PARSER_HPP
#define GCODE_PARSER_HPP

#include "motion_control.hpp"
#include "pen_control.hpp"
#include <Arduino.h>

// Xử lý luồng input: Serial G-code -> command parsing

void handleSerialInput();                // Đọc Serial, gom thành dòng G-code
void processGcodeLine(const char *line); // Phân tích dòng G-code và thực thi

#endif