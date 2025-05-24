#include "DFMobile.h"
#include "GoBLE.h"

DFMobile Robot(7, 6, 4, 5);  // Moteurs

// Variables pour les joysticks et boutons
int joystickX, joystickY;
int buttonState[7];
const int vitesseMax = 255;

int LeftWheelSpeed = 0;
int RightWheelSpeed = 0;

int modeJoystick = 0;  // 0 = directionnel, 1 = analogique

void setup() {
  Robot.Direction(LOW, LOW);
  Goble.begin();
  Serial.begin(115200);
  pinMode(13, OUTPUT); // LED test
}

void loop() {
  if (Goble.available()) {
    // Lecture des entrées
    joystickX = Goble.readJoystickX();
    joystickY = Goble.readJoystickY();

    buttonState[SWITCH_UP]     = Goble.readSwitchUp();
    buttonState[SWITCH_DOWN]   = Goble.readSwitchDown();
    buttonState[SWITCH_LEFT]   = Goble.readSwitchLeft();
    buttonState[SWITCH_RIGHT]  = Goble.readSwitchRight();
    buttonState[SWITCH_SELECT] = Goble.readSwitchSelect();  // bouton 5
    buttonState[SWITCH_START]  = Goble.readSwitchStart();   // bouton 6

    // ✅ Changement de mode
    if (buttonState[SWITCH_SELECT] == PRESSED) {
      modeJoystick = 0;  // Mode boutons
      Serial.println("Mode directionnel activé");
    } 
    else if (buttonState[SWITCH_START] == PRESSED) {
      modeJoystick = 1;  // Mode joystick analogique
      Serial.println("Mode joystick analogique activé");
    }

    // ✅ Contrôle selon le mode sélectionné
    if (modeJoystick == 0) {
      // --- Mode directionnel ---
      if (buttonState[SWITCH_UP] == PRESSED) {
        Robot.Speed(255, 255);
      } else if (buttonState[SWITCH_DOWN] == PRESSED) {
        Robot.Speed(-255, -255);
      } else if (buttonState[SWITCH_LEFT] == PRESSED) {
        Robot.Speed(-255, 255);
      } else if (buttonState[SWITCH_RIGHT] == PRESSED) {
        Robot.Speed(255, -255);
      } else {
        Robot.Speed(0, 0);
      }

    } else {
      // --- Mode analogique avec vitesse progressive ---
      int SpeedX = map(joystickX, 1, 255, -vitesseMax, vitesseMax);
      int SpeedY = map(joystickY, 1, 255, -vitesseMax, vitesseMax);

      Serial.print("SpeedX: "); Serial.print(SpeedX);
      Serial.print("  SpeedY: "); Serial.println(SpeedY);

      if (abs(SpeedX) > 20) {
        LeftWheelSpeed = map(abs(SpeedX), 20, 255, 0, vitesseMax) * (SpeedX > 0 ? 1 : -1);
        RightWheelSpeed = LeftWheelSpeed;
      } 
      else if (abs(SpeedY) > 20) {
        LeftWheelSpeed = map(abs(SpeedY), 20, 255, 0, vitesseMax) * (SpeedY > 0 ? 1 : -1);
        RightWheelSpeed = -LeftWheelSpeed;
      } 
      else {
        LeftWheelSpeed = 0;
        RightWheelSpeed = 0;
      }

      Robot.Speed(LeftWheelSpeed, RightWheelSpeed);
    }

    // --- Optionnel : test LED avec bouton UP ---
    if (buttonState[SWITCH_UP] == PRESSED) {
      digitalWrite(13, HIGH);
    } else {
      digitalWrite(13, LOW);
    }
  }
}
