#include "MotionControlService.h"
MotionControlService::MotionControlService(){}
MotionControlService& MotionControlService::getInstance(){
	static MotionControlService instance;
	return instance; }
void MotionControlService::manualMode(){
	// Code Manual Mode
}
void MotionControlService::sdcardMode(){
	// Code SD Mode
}
void MotionControlService::serialMode(){
	// Code Serial Mode
}