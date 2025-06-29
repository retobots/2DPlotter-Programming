#include "HAL/SDCardHAL.h"
#include <SD.h>

SDCardHAL::SDCardHAL(){}

SDCardHAL& SDCardHAL::getInstance(){
  static SDCardHAL instance;
  return instance;
}

void SDCardHAL::setup(){
  if(!SD.begin(5)){
    Serial.println("Loi mo SD!");
  }
}

std::vector<std::string> SDCardHAL::listFiles(){
  std::vector<std::string> files;
  File root=SD.open("/");
  if(!root){
    return files;
  }
  while(true){
    File entry=root.openNextFile();
    if(!entry){
      break;
    }
    if(!entry.isDirectory()){
      std::string name=entry.name();
      if(name.size()>6&&name.substr(name.size()-6)==".gcode"&&name.size()<=32){
        files.push_back(name);
      }
    }
    entry.close();
  }
  root.close();
  return files;
}

bool SDCardHAL::openFile(const std::string& filename){
  if(filename.size()>32){
    return false;
  }
  if(currentFile){
    currentFile.close();
  }
  currentFile=SD.open(filename.c_str(), FILE_READ);
  if(!currentFile||currentFile.size()>64*1024){
    if(currentFile){
      currentFile.close();
    }
    return false;
  }
  return true;
}

std::string SDCardHAL::readLine(){
  if(!currentFile){
    return "";
  }
  String line=currentFile.readStringUntil('\n');
  if(line.length()>80){
    return "";
  }
  return std::string(line.c_str());
}

void SDCardHAL::closeFile(){
  if(currentFile){
    currentFile.close();
  }
}

/* HDSD:
#include "SDCardHAL.h"

void setup(){
  Serial.begin(115200);
  SDCardHAL::getInstance().setup();
  std::string filename = "example.gcode";
  if(SDCardHAL::getInstance().openFile(filename)){
    Serial.println("Đang đọc file...");
    std::string line;
    while(!(line = SDCardHAL::getInstance().readLine()).empty()){
      Serial.println(line.c_str());
      // Thêm logic xử lý dòng ở đây nếu cần
    }
    SDCardHAL::getInstance().closeFile();
    Serial.println("Đã đọc xong file.");
  }else{
    Serial.println("Không thể mở file!");
  }
}
void loop(){
}*/