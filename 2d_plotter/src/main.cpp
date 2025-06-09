#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int backlightPin = 15; // Chan backlight LCD
const int encoderPinA = 26; // Chan A encoder
const int encoderPinB = 27; // Chan B encoder
const int buttonPin = 25; // Chan button
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define MAIN_MENU 0
#define AUTO_MENU 1
int currentMenu=MAIN_MENU;
int topIndex=0;
int selectedIndex=0;
// menu options
const char* mainMenuOptions[]={"Auto Mode", "Manual Mode"};
const int mainNumOptions = 2;
const char* autoMenuOptions[]={"SD Card", "Serial", "Back"};
const int autoNumOptions=3;
const char** options; 
int numOptions;

void setup(){
  Wire.begin();
  lcd.init();
  lcd.backlight();
  pinMode(backlightPin, OUTPUT);
  digitalWrite(backlightPin, LOW);
  lcd.print("RETOBOTS");
  for (int i=0; i<=255; i+=5){
    analogWrite(backlightPin, i);
    delay(20);
  }
  delay(1000);
  for (int i=255; i>=0; i-=5){
    analogWrite(backlightPin, i);
    delay(20);
  }
  lcd.clear();
  digitalWrite(backlightPin, HIGH);
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderPinA), encoderISR, RISING);
  currentMenu=MAIN_MENU;
  options=mainMenuOptions;
  numOptions=mainNumOptions;
  topIndex=0;
  selectedIndex = 0;
}

void loop(){
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
        manualMode();
        currentMenu=MAIN_MENU;
        options=mainMenuOptions;
        numOptions=mainNumOptions;
        topIndex=0;
        selectedIndex=0;
      }
    }else if(currentMenu==AUTO_MENU){
      if(selectedIndex==0){
        sdCardMode();
        currentMenu=MAIN_MENU;
        options=mainMenuOptions;
        numOptions=mainNumOptions;
        topIndex=0;
        selectedIndex=0;
      }else if(selectedIndex==1){
        serialMode();
        currentMenu=MAIN_MENU;
        options=mainMenuOptions;
        numOptions=mainNumOptions;
        topIndex=0;
        selectedIndex=0;
      } else if(selectedIndex==2){
        currentMenu=MAIN_MENU;
        options=mainMenuOptions;
        numOptions=mainNumOptions;
        topIndex=0;
        selectedIndex=0;
      }
    }
    delay(200);
  }
}
void displayMenu(){
  lcd.clear();
  int displayRows=2;
  for(int i=0; i<displayRows; i++){
    int optionIndex=topIndex + i;
    if(optionIndex<numOptions){
      lcd.setCursor(0, i);
      if(optionIndex==selectedIndex){
        lcd.print("> ");
      }else{
        lcd.print("  ");
      }
      lcd.print(options[optionIndex]);
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
void encoderISR(){ // Cuon menu
  int direction=digitalRead(encoderPinB)==HIGH?1:-1;
  int newIndex=selectedIndex+direction;
  if(newIndex>=0&&newIndex<numOptions){
    selectedIndex=newIndex;
    if(selectedIndex<topIndex){
      topIndex=selectedIndex;
    } else if(selectedIndex > topIndex+1){
      topIndex=selectedIndex-1;
    }
  }
}
void manualMode(){
  // Manual
}

void sdCardMode(){
  // SD Card
}

void serialMode(){
  // Serial mode
}
