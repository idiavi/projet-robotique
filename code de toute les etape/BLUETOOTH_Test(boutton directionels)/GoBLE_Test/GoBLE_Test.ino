#include <Metro.h>
#include "GoBLE.h"

int joystickX, joystickY;
int buttonState[7];
int speedPin_M1 = 5;     // M1 Speed Control
int speedPin_M2 = 6;     // M2 Speed Control
int directionPin_M1 = 4; // M1 Direction Control
int directionPin_M2 = 7; // M2 Direction Control


void setup(){
  Goble.begin();
  Serial.begin(115200);
}

void loop() {  
  if(Goble.available()){
    joystickX = Goble.readJoystickX();
    joystickY = Goble.readJoystickY();
    
    buttonState[SWITCH_UP]     = Goble.readSwitchUp();
    buttonState[SWITCH_DOWN]   = Goble.readSwitchDown();
    buttonState[SWITCH_LEFT]   = Goble.readSwitchLeft();
    buttonState[SWITCH_RIGHT]  = Goble.readSwitchRight();
    buttonState[SWITCH_SELECT] = Goble.readSwitchSelect();
    buttonState[SWITCH_START]  = Goble.readSwitchStart();
  
    Serial.print("Joystick Value: ");
    Serial.print("  ");
    Serial.println(joystickY);
  
    
    //for (int i = 1; i < 7; i++) {
      //Serial.print("Button ID: ");
      //Serial.print(i);
// 📌 Vérification des boutons
if (buttonState[SWITCH_UP] == PRESSED) {
    carAdvance(255,255);
} else if (buttonState[SWITCH_DOWN] == PRESSED) {
    carBack(255,255);
} else if (buttonState[SWITCH_LEFT] == PRESSED) {
    carTurnLeft(255,255);
} else if (buttonState[SWITCH_RIGHT] == PRESSED) {
    carTurnRight(255,255);
} else {
    carStop();  // Si aucun bouton n'est appuyé
}




    //}
  } 
} 
void carStop() {
  analogWrite(speedPin_M1, 0);
  digitalWrite(directionPin_M1, LOW);
  analogWrite(speedPin_M2, 0);
  digitalWrite(directionPin_M2, LOW);
}

void carBack(int leftSpeed, int rightSpeed) {
  analogWrite(speedPin_M2, leftSpeed);
  digitalWrite(directionPin_M1, LOW);
  analogWrite(speedPin_M1, rightSpeed);
  digitalWrite(directionPin_M2, HIGH);
}

void carAdvance(int leftSpeed, int rightSpeed) {
  analogWrite(speedPin_M2, leftSpeed);
  digitalWrite(directionPin_M1, HIGH);
  analogWrite(speedPin_M1, rightSpeed);
  digitalWrite(directionPin_M2, LOW);
}

void carTurnLeft(int leftSpeed, int rightSpeed) {
  analogWrite(speedPin_M2, leftSpeed);
  digitalWrite(directionPin_M1, HIGH);
  analogWrite(speedPin_M1, rightSpeed);
  digitalWrite(directionPin_M2, HIGH);
}

void carTurnRight(int leftSpeed, int rightSpeed) {
  analogWrite(speedPin_M2, leftSpeed);
  digitalWrite(directionPin_M1, LOW);
  analogWrite(speedPin_M1, rightSpeed);
  digitalWrite(directionPin_M2, LOW);
}


