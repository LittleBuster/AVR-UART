#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  pinMode(2, OUTPUT);
  delay(10);
  digitalWrite(2, LOW); //recv data
  Serial.begin(9600);
  
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

String inputString = "";
boolean stringComplete = false;

void recv() {
  if (stringComplete) {
    lcd.setCursor(0, 0);
    lcd.print(inputString);

    inputString = "";
    stringComplete = false;
  }
}


void loop() {
  recv();
  delay(1000);
}

//Прерывание сериала
void serialEvent() {
  while (Serial.available()) {    
    char inChar = (char)Serial.read();
    inputString += inChar;
    
    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
}