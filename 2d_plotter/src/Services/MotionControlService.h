#ifndef RobotControlService_h
#define RobotControlService_h
class RobotControlService {
public:
	static RobotControlService& getInstance();
	void manualMode();
	void sdCardMode();
	void serialMode();
private:
	RobotControlService();
};
#endif