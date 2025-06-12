#include "UIMenuService.h"
UIMenuService* UIMenuService::instance = nullptr; //Singleton de xu ly truong hop khong co instance
const char* UIMenuService::mainMenuOptions[]={ "Auto Mode", "Manual Mode" };
const char* UIMenuService::autoMenuOptions[]={ "SD Card", "Serial", "Back" };
UIMenuService::UIMenuService() : lcd(0x27, 16, 2){
    instance = this;
}
// Singleton pattern
UIMenuService& UIMenuService::getInstance(){
    static UIMenuService instance; return instance; }
// Ham khoi tao LCD va cac thiet bi
void UIMenuService::setup(){
    Wire.begin();
    lcd.init();
    lcd.backlight();
    pinMode(backlightPin, OUTPUT);
    digitalWrite(backlightPin, LOW);
    lcd.print("RETOBOTS");
    for(int i=0;i<=255;i+=5){
        analogWrite(backlightPin, i);
        delay(20);
    }
    delay(1000);
    for(int i=255;i>=0;i-=5){
        analogWrite(backlightPin, i);
        delay(20);
    }
    lcd.clear();
    digitalWrite(backlightPin, HIGH);
    pinMode(encoderPinA, INPUT_PULLUP);
    pinMode(encoderPinB, INPUT_PULLUP);
    pinMode(buttonPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(encoderPinA), encoderISR, RISING);
    currentMenu = MAIN_MENU;
    options = mainMenuOptions;
    numOptions = mainNumOptions;
    topIndex = 0;
    selectedIndex = 0;
}
ModeType UIMenuService::selectMode(){
    while(true){
        displayMenu();
        if(digitalRead(buttonPin)==LOW){
            if(currentMenu==MAIN_MENU){
                if(selectedIndex==0){
                    currentMenu=AUTO_MENU;
                    options=autoMenuOptions;
                    numOptions=autoNumOptions;
                    topIndex=0;
                    selectedIndex=0;
                }else if(selectedIndex==1){
                    return ModeType::MANUAL_MODE; }// Manual Mode
            }else if(currentMenu==AUTO_MENU){
                if(selectedIndex==0){
					return ModeType::SD_CARD; // SD Card Mode
                }else if(selectedIndex==1){
					return ModeType::SERIAL; // Serrial Mode
                }else if(selectedIndex==2){ //Cau hinh nut "Back"
                    currentMenu=MAIN_MENU;
                    options=mainMenuOptions;
                    numOptions=mainNumOptions;
                    topIndex=0;
                    selectedIndex=0;
                }
            } delay(200);
        } delay(10);
    } return ModeType::NONE;
}
void UIMenuService::displayMenu(){
    lcd.clear();
    int displayRows=2;
    for(int i=0;i<displayRows;i++){
        int optionIndex=topIndex+i;
        if(optionIndex<numOptions){
            lcd.setCursor(0, i);
            if(optionIndex==selectedIndex){
                lcd.print("> ");
            }else{
                lcd.print("  ");
            } lcd.print(options[optionIndex]);
        }
    }
    if(topIndex>0){
        lcd.setCursor(15, 0);
        lcd.print("^");
    }
    if(topIndex+displayRows<numOptions){
        lcd.setCursor(15, 1);
        lcd.print("v");
    }
}
void UIMenuService::encoderISR(){
    if(instance){
        int direction=digitalRead(encoderPinB)==HIGH?1:-1;
        int newIndex=instance->selectedIndex+direction;
        if(newIndex>=0&&newIndex<instance->numOptions){
            instance->selectedIndex=newIndex;
            if(instance->selectedIndex<instance->topIndex){
                instance->topIndex=instance->selectedIndex;
            }else if(instance->selectedIndex>instance->topIndex+1){
                instance->topIndex=instance->selectedIndex-1;
            }
        }
    }
}

/* 
*/