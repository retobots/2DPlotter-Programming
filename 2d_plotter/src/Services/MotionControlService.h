#ifndef MotionControlService_h
#define MotionControlService_h
class MotionControlService{
public:
	static MotionControlService& getInstance();
	void manualMode();
	void sdcardMode();
	void serialMode();
private:
	MotionControlService();
};
#endif
