#include <LiquidCrystal_I2C.h>
#include <math.h>

// Sensor & Display Setup
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
int irSensorPinX_0 = 2;//XY sensor pins; 6 each, 3 front, 3 back
int irSensorPinX_1;
int irSensorPinX_2;
int irSensorPinX_3;
int irSensorPinX_4;
int irSensorPinX_5;
int irSensorPinY_0;
int irSensorPinY_1;
int irSensorPinY_2;
int irSensorPinY_3;
int irSensorPinY_4;
int irSensorPinY_5;
typedef enum {nothingIn, somethingIn, state3} state;
state currentState = nothingIn;

//Calculation setup - Placeholder values
const float dz = 1; //Distance variables
float x_0, x_1, y_0, y_1, dx, dy;
float t_0, t_1, dt, t_last; //Time variables
int ball_num;//Ball info variables
float bpm;
float V;
void setup() {
  Serial.begin(9600);
  pinMode(irSensorPinX_0, INPUT);
  pinMode(irSensorPinX_1, INPUT);
  pinMode(irSensorPinX_2, INPUT);
  pinMode(irSensorPinX_3, INPUT);
  pinMode(irSensorPinX_4, INPUT);
  pinMode(irSensorPinX_5, INPUT);
  pinMode(irSensorPinY_0, INPUT);
  pinMode(irSensorPinY_1, INPUT);
  pinMode(irSensorPinY_2, INPUT);
  pinMode(irSensorPinY_3, INPUT);
  pinMode(irSensorPinY_4, INPUT);
  pinMode(irSensorPinY_5, INPUT);
  lcd.init();                      
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Test start");
  delay(1000);
  lcd.setCursor(0,0);
  lcd.print("            ");
}

void updateBPM() {
  if(millis()%5000 == 0) {
    bpm = (ball_num/millis())*60000;//(number of balls/time in ms)*(ms in a minute)
    //time can be changed to start from the first time a ball passes through, as opposed to the arduino starting
    lcd.setCursor(0,0);
    lcd.print("BPM: ");
    lcd.print(bpm);
  }
}
float new_ball() {
  lcd.setCursor(0,0);
  lcd.print(ball_num);
  lcd.print(" Balls");
  lcd.setCursor(0,1);
  lcd.print(V);
  lcd.print("in/s"); //Inches per second, but can be made to m/s, but everything has to be converted.
  return millis();
}

float IR_init_check_X() {//checks for the front-most IR sensors
  if(digitalRead(irSensorPinX_0) == LOW){
    return 1.4; //inches from wall
  }
  else if(digitalRead(irSensorPinX_1) == LOW){
    return 2.9;
  }
  else if(digitalRead(irSensorPinX_2) == LOW){
    return 4.4; //inches from wall
  }
  //add more of these conditions as appropriate
  else{
    return 0.0;
  }
}
float IR_init_check_Y() {
  if(digitalRead(irSensorPinY_0) == LOW){
    return 1.4; //inches from wall
  }
  else if(digitalRead(irSensorPinY_1) == LOW){
    return 2.9;
  }
  else if(digitalRead(irSensorPinY_2) == LOW){
    return 4.4;
  }
  //add more of these conditions as appropriate
  else{
    return 0.0;
  }
}
float IR_fin_check_X() {//checks for backmost IR sensors
  if(digitalRead(irSensorPinX_3) == LOW){
    return 1.4; //inches from wall
  }
  else if(digitalRead(irSensorPinX_4) == LOW){
    return 2.9;
  }
  else if(digitalRead(irSensorPinX_5) == LOW){
    return 4.4;
  }
  //add more of these conditions as appropriate
  else{
    return 0.0;
  }
}
float IR_fin_check_Y() {
  if(digitalRead(irSensorPinY_3) == LOW){
    return 1.4; //inches from wall
  }
  else if(digitalRead(irSensorPinY_4) == LOW){
    return 2.9;
  }
  else if(digitalRead(irSensorPinY_5) == LOW){
    return 4.4;
  }
  //add more of these conditions as appropriate
  else{
    return 0.0;
  }
}

void loop() {
  while((IR_init_check_X() == 0)||(IR_init_check_Y() == 0)){
    if (millis()<=t_last+1000) {
      updateBPM();
    }
  }
  t_0 = millis();
  x_0 = IR_init_check_X();
  y_0 = IR_init_check_Y();
  while(((IR_fin_check_X() == 0)||(IR_fin_check_Y() == 0))&&(millis()<=t_0+300)){
    updateBPM();
  }
  if(millis()<=t_0+300){
    t_1 = millis();
    x_1 = IR_fin_check_X();
    y_1 = IR_fin_check_Y();
    ball_num++;
    dt = t_1-t_0;
    dx = x_1-x_0;
    dy = y_1-y_0;
    V = (sqrt(dx*dx + dy*dy + dz*dz))/dt;
    t_last = new_ball();
  }
  else{;}
}

/*void loop() {
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
}*/