#ifndef UIMenuService_h
#define UIMenuService_h
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
enum class ModeType{
	NONE,
	MANUAL_MODE,
	SD_CARD,
	SERIAL
};
class UIMenuService{
public:
	static UIMenuService& getInstance();
	void setup();
	ModeType selectMode();
private:
	UIMenuService();
	static UIMenuService* instance; // Truy cap instance trong ISR
	LiquidCrystal_I2C lcd;
	int currentMenu; // Menu hien thi
	int topIndex;
	int selectedIndex;
	const char** options; //danh sach option trong menu
	int numOptions; // So luong tuy chon trong menu
	// Pin
	static const int backlightPin=15;
	static const int encoderPinA=26;
	static const int encoderPinB=27;
	static const int buttonPin=25;
	static const char* mainMenuOptions[];
	static const int mainNumOptions=2;
	static const char* autoMenuOptions[];
	static const int autoNumOptions=3;
	// Menu trong option:
	static const int MAIN_MENU=0;
	static const int AUTO_MENU=1;
	void displayMenu();
	static void encoderISR();  // ham ngatxu ly encoder de doc encoder
};
#endif