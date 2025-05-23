#ifndef SDREADER_H
#define SDREADER_H

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

class SDReader{
public:
    SDReader(uint8_t csPin); // Constructor:truyen chan CS of SD
    bool begin();   //hmmm
    File openFirstTxtFile();    // mo file .txt DAU TIEN(first)
    String readLine(File &file);   // doc tung dong den khi thay \n
    char readChar(File &file);  // Doc chu
    long readInt(File &file);   // Doc int
    float readFloat(File &file);    // Doc float
private:
    uint8_t _csPin;
};
#endif