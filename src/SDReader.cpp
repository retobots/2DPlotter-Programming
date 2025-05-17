#include "SDReader.h"

SDReader::SDReader(uint8_t csPin):_csPin(csPin){}
bool SDReader::begin(){
    pinMode(_csPin, OUTPUT);
    if(!SD.begin(_csPin)){
        Serial.println("Mo the SD fail roi!");
        return false;
    }
    return true;
}
File SDReader::openFirstTxtFile(){
    File root=SD.open("/");
    if(!root){
        Serial.println("That bai trong viec doc thu muc!");
        return File();
    }
    File file=root.openNextFile();
    while(file){ // Check .txt
        String name=file.name();
        if (name.endsWith(".txt")||name.endsWith(".TXT")){
            Serial.print("File: "); 
            Serial.println(name);
            return file;  // Tra ve file dau tien tim thay
        }
        file=root.openNextFile();
    }
    Serial.println("Ko tim thay file .txt nao!");
    return File();
}
String SDReader::readLine(File &file){ // Doc tung hang
    String line="";
    while(file.available()){
        char c=file.read();
        if (c=='\n') break;
        if (c!='\r') line+=c;
    }
    return line;
}
char SDReader::readChar(File &file){
    if(file.available()){
        return file.read();
    }
    return '\0';
}
long SDReader::readInt(File &file){ // Doc so nguyen
    String numStr="";
    while(file.available()) {
        char c=file.peek(); 
        if(isdigit(c)||c =='-'||c=='+'){
            numStr+=file.read();
        }else break;
    }
    return numStr.toInt();
}
float SDReader::readFloat(File &file){ // Doc so thuc
    String numStr="";
    while(file.available()){
        char c=file.peek();
        if(isdigit(c)||c=='-'||c=='+'||c=='.'){
            numStr+=file.read();
        }else break;
    }
    return numStr.toFloat();
}
