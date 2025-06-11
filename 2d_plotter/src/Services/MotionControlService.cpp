#include "MotionControlService.h"
MotionControlService::MotionControlService(){}
MotionControlService& MotionControlService::getInstance(){
	static MotionControlService instance;
	return instance; }
void MotionControlService::startManualMode(){
	// Code Manual Mode
}
void MotionControlService::loadFromSDCard(){
	// Code SD Mode
}
void MotionControlService::connectSerial(){
	// Code Serial Mode
}
