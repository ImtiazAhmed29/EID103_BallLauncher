#include <LiquidCrystal_I2C.h>
#include <math.h>
  
// Sensor & Display Setup
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
int ButtonPin = 1;
int irSensorPin_0 = 2;//XY sensor pins; 6 each, 3 front, 3 back
int irSensorPin_1 = 4;
int irSensorPin_2 = 6;
int irSensorPin_3 = 8;
int irSensorPin_4 = 3;
int irSensorPin_5 = 5;
int irSensorPin_6 = 7;
int irSensorPin_7 = 9;
int MotorPin_1 = 11;
int MotorPin_2 = 12;
int MotorPin_3 = 13;
typedef enum {nothingIn, somethingIn, state3} state;
state currentState = nothingIn;

//Calculation setup - Placeholder values
const float dz = 1.347; //Distance variables
float x_0, x_1, y_0, y_1, dx, dy;
float t_start, t_0, t_1, dt, t_last; //Time variables
int t_press_1 = 0
int t_press_2 = 0
float ball_num=0;//Ball info variables
double bpm;
double bps;
long V;
void setup() {
  Serial.begin(9600);
  pinMode(irSensorPin_0, INPUT);
  pinMode(irSensorPin_1, INPUT);
  pinMode(irSensorPin_2, INPUT);
  pinMode(irSensorPin_3, INPUT);
  pinMode(irSensorPin_4, INPUT);
  pinMode(irSensorPin_5, INPUT);
  pinMode(irSensorPin_6, INPUT);
  pinMode(irSensorPin_7, INPUT);
  lcd.init();                      
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Test start");
  delay(1000);
  lcd.setCursor(0,0);
  lcd.clear();
  lcd.print("Press Button to Start");
  while(digitalRead(ButtonPin) == 1)
  {
    ;
  }
  lcd.clear();
  lcd.print("Start!");
  t_last = millis();
}

void updateBPM() {
  if(millis()%5000 == 0) {
    bps = (ball_num)/((millis()-t_last)/1000);//(number of balls/time in ms)*(ms in a minute)
    bpm = bps*60;
    //I increased the precision of bpm from float to double, os hopefully it rounds the time a lot less
    //time can be changed to start from the first time a ball passes through, as opposed to the arduino starting
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("BPM: ");
    lcd.print(bpm);
    lcd.print("      ");
    Serial.println(bpm);
  }
}
int new_ball() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(ball_num);
  lcd.print(" Balls");
  lcd.setCursor(0,1);
  lcd.print(V);
  lcd.print("in/s"); //Inches per second, but can be made to m/s, but everything has to be converted.
  lcd.print(" | ");
  lcd.print(dt);
  return millis();
}

float IR_init_check() {//checks for the front-most IR sensors
  float dist = 0.0;
  float num = 0;
  if(digitalRead(irSensorPin_0) == LOW){
    dist += 0.901; //inches from wall
    num++;
  }
  if(digitalRead(irSensorPin_1) == LOW){
    dist += 2.234;
    num++;
  }
  if(digitalRead(irSensorPin_2) == LOW){
    dist += 3.567;
    num ++; //inches from wall
  }
  if(digitalRead(irSensorPin_3) == LOW){
    dist += 4.901;
    num++ ;//inches from wall
  }
  if(num>0){
    dist /= num;
  }
  return dist;
}
float IR_fin_check() {//checks for the back-most IR sensors
  float dist = 0.0;
  float num = 0;
  if(digitalRead(irSensorPin_4) == LOW){
    dist += 0.901; //inches from wall
    num++;
  }
  if(digitalRead(irSensorPin_5) == LOW){
    dist += 2.234;
    num++;
  }
  if(digitalRead(irSensorPin_6) == LOW){
    dist += 3.567;
    num ++; //inches from wall
  }
  if(digitalRead(irSensorPin_7) == LOW){
    dist += 4.901;
    num++;  //inches from wall
  }
  if(num>0){
    dist /= num;
  }
  return dist;
}

void loop() {
  while((y_0 = (IR_init_check()) == 0)){
    if (millis()>=t_last+5000) {
      updateBPM();
      if((digitalRead(ButtonPin) != 0)&&(t_press_1 == 0)){
        t_press_1 = millis();
      }
      else if (digitalRead(ButtonPin) != 0)&&(millis >= t_press_1+50)){
        t_press_1 = 0;
        goto buttonlock;
      }
      else if (digitalRead(ButtonPin) != 0)&&(millis <= t_press_1+50)){
        ;
      }
      else{
        t_press_1 = 0;
      }
    }
  }
  t_0 = millis();
  y_0 = IR_init_check();
  while(((IR_fin_check() == 0))&&(millis()<=t_0+300)){
    updateBPM();
    if((digitalRead(ButtonPin) != 0)&&(t_press_1 == 0)){
        t_press_1 = millis();
      }
      else if (digitalRead(ButtonPin) != 0)&&(millis >= t_press_1+50)){
        t_press_1 = 0;
        goto buttonlock;
      }
      else if (digitalRead(ButtonPin) != 0)&&(millis <= t_press_1+50)){
        ;
      }
      else{
        t_press_1 = 0;
      }
  }
  if(millis()<=t_0+301){
    t_1 = millis();
    y_1 = IR_fin_check();
    ball_num++;
    dt = t_1-t_0;
    dy = y_1-y_0;
    V = 1000*(sqrt(dy*dy + dz*dz))/dt;
    new_ball();
    t_last = millis();
  }
  else{;}
  while (((IR_fin_check() != 0)))
  {
    updateBPM();
    if((digitalRead(ButtonPin) != 0)&&(t_press_1 == 0)){
        t_press_1 = millis();
      }
      else if (digitalRead(ButtonPin) != 0)&&(millis >= t_press_1+50)){
        t_press_1 = 0;
        goto buttonlock;
      }
      else if (digitalRead(ButtonPin) != 0)&&(millis <= t_press_1+50)){
        ;
      }
      else{
        t_press_1 = 0;
      }
  }
  goto skip;
  buttonlock:
  lcd.clear();
  lcd.print("Hoop reset");
  lcd.setCursor(0, 1)
  lcd.print("Press Again to Start")
  bpm = 0;
  ball_num = 0;
  t_last = 0;
  delay(3000)//Very specifically for interupting this
  while((digitalRead(ButtonPin) != 0))
  {
    ;
  }

  skip:
  ;
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
