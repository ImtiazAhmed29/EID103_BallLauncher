#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
int irSensorPin_0 = 2;
typedef enum {nothingIn, somethingIn, state3} state;
state currentState = nothingIn;

void setup() {
  Serial.begin(9600);
  pinMode(irSensorPin_0, INPUT);
  lcd.init();                      
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Test start");
  delay(1000);
  lcd.setCursor(0,0);
  lcd.print("            ");
}

void loop() {
  if (digitalRead(irSensorPin_0) == HIGH) {
    Serial.println("Nothing detected");
    Serial.print(currentState); //debugging
    while (currentState == somethingIn) {
      lcd.setCursor(0,0);
      lcd.print("Nothing in");
      currentState = nothingIn;
    }
    //
  }
  else if (digitalRead(irSensorPin_0) == LOW) {
    Serial.println("Object detected");
    Serial.print(currentState);
    while (currentState == nothingIn) {
      lcd.setCursor(0,0);
      lcd.print("Something went in");
      currentState = somethingIn;
    }
  }
}
